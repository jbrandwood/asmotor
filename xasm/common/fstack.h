/*  Copyright 2008 Carsten S�rensen

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

#ifndef	INCLUDE_FSTACK_H
#define	INCLUDE_FSTACK_H

typedef	enum
{
	CONTEXT_FILE,
	CONTEXT_REPT,
	CONTEXT_MACRO
} EContextType;

struct FileStack
{
	list_Data(struct FileStack);
	char*			pName;
	SLexBuffer*		pLexBuffer;
	SLONG			LineNumber;
	EContextType	Type;
	char*			RunID;	/*	For the \@ symbol */

	/*	This is for repeating block type stuff. Currently only REPT */
	union
	{
		struct
		{
			char*	pOriginalBuffer;
			ULONG	OriginalSize;
			ULONG	RemainingRuns;
		} Rept;
		struct
		{
			char*	Arg0;
			char**	Args;
			ULONG	ArgCount;
		} Macro;
	} BlockInfo;
};
typedef struct FileStack SFileStack;

extern void fstk_RunMacro(char* symname);
extern void fstk_RunInclude(char* s);
extern BOOL fstk_RunNextBuffer(void);
extern BOOL fstk_Init(char* s);
extern void fstk_Cleanup(void);
extern void fstk_Dump(void);
extern void fstk_FindFile(char* *s);
extern void fstk_RunRept(char* buffer, ULONG size, ULONG count);
extern char* fstk_GetMacroArgValue(char ch);
extern char* fstk_GetMacroRunID(void);
extern void fstk_AddMacroArg(char* s);
extern void fstk_SetMacroArg0(char* s);
extern void fstk_ShiftMacroArgs(SLONG count);
extern SLONG fstk_GetMacroArgCount(void);
extern void fstk_AddIncludePath(char* s);

extern SFileStack* g_pFileContext;

#endif	/*INCLUDE_FSTACK_H*/