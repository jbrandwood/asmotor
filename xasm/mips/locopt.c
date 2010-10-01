/*  Copyright 2008 Carsten Sørensen

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

#include "../common/xasm.h"

#include "loccpu.h"

void locopt_Open(void)
{
	pOptions->Machine.nCpu = CPUF_MIPS32R2;
}

void locopt_Update(void)
{
}

BOOL locopt_Parse(char* s)
{
	if(s == NULL || strlen(s) == 0)
		return FALSE;

	switch(s[0])
	{
		case 'c':
			if(strlen(&s[1]) == 1)
			{
				switch(s[1] - '0')
				{
					case 0:
						pOptions->Machine.nCpu = CPUF_MIPS32R1;
						return TRUE;
					case 1:
						pOptions->Machine.nCpu = CPUF_MIPS32R2;
						return TRUE;
					default:
						prj_Warn(WARN_MACHINE_UNKNOWN_OPTION, s);
						return FALSE;
				}
			}
			else
			{
				prj_Warn(WARN_MACHINE_UNKNOWN_OPTION, s);
				return FALSE;
			}
			break;
		default:
			prj_Warn(WARN_MACHINE_UNKNOWN_OPTION, s);
			return FALSE;
	}
}

void locopt_PrintOptions(void)
{
	printf("    -mc<X>  Enable CPU:\n");
	printf("            0 = MIPS32 R1\n");
	printf("            1 = MIPS32 R2 (default)\n");
}