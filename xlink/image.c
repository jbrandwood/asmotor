/*  Copyright 2008-2015 Carsten Elton Sorensen

    This file is part of ASMotor.

    ASMotor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ASMotor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ASMotor.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xlink.h"
#include <memory.h>

#define WRITE_BLOCK_SIZE 16384

static void writeRepeatedBytes(FILE* fileHandle, void* data, uint32_t offset, int bytes)
{
	fseek(fileHandle, offset, SEEK_SET);
	while (bytes > 0)
	{
		uint32_t towrite = bytes > WRITE_BLOCK_SIZE ? WRITE_BLOCK_SIZE : bytes;
		if (towrite != fwrite(data, 1, towrite, fileHandle))
			Error("Disk possibly full");
		bytes -= towrite;
	}
}

static void* allocEmptyBytes()
{
	void* data = (char*)mem_Alloc(WRITE_BLOCK_SIZE);
	if (data == NULL)
		Error("Out of memory");

	memset(data, 0, WRITE_BLOCK_SIZE);

	return data;
}


extern void image_WriteBinary(char* outputFilename)
{
	char* emptyBytes = allocEmptyBytes();

	FILE* fileHandle;
	uint32_t currentFileSize = 0;

	fileHandle = fopen(outputFilename, "wb");
	if (fileHandle == NULL)
		Error("Unable to open \"%s\" for writing", outputFilename);

	for (Section* section = g_sections; section != NULL; section = section->nextSection)
	{
		//	This is a special exported EQU symbol section
		if (section->group == NULL)
			continue;

		if (section->used && section->assigned && section->imageLocation != -1)
		{
			uint32_t startOffset = section->imageLocation;
			uint32_t endOffset = startOffset + section->size;

			if(startOffset > currentFileSize)
				writeRepeatedBytes(fileHandle, emptyBytes, startOffset, startOffset - currentFileSize);

			fseek(fileHandle, section->imageLocation, SEEK_SET);
			fwrite(section->data, 1, section->size, fileHandle);
			if (endOffset > currentFileSize)
				currentFileSize = endOffset;
		}
	}

	fclose(fileHandle);
	mem_Free(emptyBytes);
}