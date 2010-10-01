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

static SLexInitString localstrings[]=
{
	"adc",	T_6502_ADC,
	"and",	T_6502_AND,
	"asl",	T_6502_ASL,
	"bit",	T_6502_BIT,

	"bpl",	T_6502_BPL,
	"bmi",	T_6502_BMI,
	"bvc",	T_6502_BVC,
	"bvs",	T_6502_BVS,
	"bcc",	T_6502_BCC,
	"bcs",	T_6502_BCS,
	"bne",	T_6502_BNE,
	"beq",	T_6502_BEQ,

	"brk",	T_6502_BRK,
	"cmp",	T_6502_CMP,
	"cpx",	T_6502_CPX,
	"cpy",	T_6502_CPY,
	"dec",	T_6502_DEC,
	"eor",	T_6502_EOR,

	"clc",	T_6502_CLC,
	"sec",	T_6502_SEC,
	"cli",	T_6502_CLI,
	"sei",	T_6502_SEI,
	"clv",	T_6502_CLV,
	"cld",	T_6502_CLD,
	"sed",	T_6502_SED,

	"inc",	T_6502_INC,
	"jmp",	T_6502_JMP,
	"jsr",	T_6502_JSR,
	"lda",	T_6502_LDA,
	"ldx",	T_6502_LDX,
	"ldy",	T_6502_LDY,
	"lsr",	T_6502_LSR,
	"nop",	T_6502_NOP,
	"ora",	T_6502_ORA,

	"tax",	T_6502_TAX,
	"txa",	T_6502_TXA,
	"dex",	T_6502_DEX,
	"inx",	T_6502_INX,
	"tay",	T_6502_TAY,
	"tya",	T_6502_TYA,
	"dey",	T_6502_DEY,
	"iny",	T_6502_INY,

	"rol",	T_6502_ROL,
	"ror",	T_6502_ROR,
	"rti",	T_6502_RTI,
	"rts",	T_6502_RTS,
	"sbc",	T_6502_SBC,
	"sta",	T_6502_STA,

	"txs",	T_6502_TXS,
	"tsx",	T_6502_TSX,
	"pha",	T_6502_PHA,
	"pla",	T_6502_PLA,
	"php",	T_6502_PHP,
	"plp",	T_6502_PLP,

	"stx",	T_6502_STX,
	"sty",	T_6502_STY,

	"a",	T_6502_REG_A,
	"x",	T_6502_REG_X,
	"y",	T_6502_REG_Y,

	NULL, 0
};

void	loclexer_Init(void)
{
	lex_AddStrings(localstrings);
}