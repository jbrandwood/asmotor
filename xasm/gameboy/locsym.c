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

#include "../common/xasm.h"

void locsym_Init(void)
{
	sym_AddGROUP("HOME", GROUP_TEXT);
	sym_AddGROUP("CODE", GROUP_TEXT);
	sym_AddGROUP("DATA", GROUP_TEXT);
	sym_AddGROUP("BSS", GROUP_BSS);
	sym_AddGROUP("HRAM", GROUP_BSS);
	sym_AddGROUP("VRAM", GROUP_BSS);
}