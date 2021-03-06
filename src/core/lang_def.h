/*
 * lang_def.h
 * Copyright (C) 2013 David Jolly
 * ----------------------
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LANG_DEF_H_
#define LANG_DEF_H_

#include <set>
#include "type.h"

enum {
	ADDRESSING_MODE_REGISTER = 0,
	ADDRESSING_MODE_INDIRECT_REGISTER,
	ADDRESSING_MODE_INDIRECT_REGISTER_NEXT_WORD,
	ADDRESSING_MODE_PUSH_POP,
	ADDRESSING_MODE_PEEK,
	ADDRESSING_MODE_PICK,
	ADDRESSING_MODE_SP_LITERAL,
	ADDRESSING_MODE_PC_LITERAL,
	ADDRESSING_MODE_EX_LITERAL,
	ADDRESSING_MODE_INDIRECT_NEXT_WORD,
	ADDRESSING_MODE_NEXT_WORD_LITERAL,
	ADDRESSING_MODE_LITERAL,
};

enum {
	DIRECTIVE_DATA = 0,
	DIRECTIVE_INCBIN,
	DIRECTIVE_INCLUDE,
	DIRECTIVE_RESERVE,
};

enum {
	OPCODE_BASIC_RESERVED = 0,
	OPCODE_BASIC_SET_CMD,
	OPCODE_BASIC_ADD,
	OPCODE_BASIC_SUB,
	OPCODE_BASIC_MUL,
	OPCODE_BASIC_MLI,
	OPCODE_BASIC_DIV,
	OPCODE_BASIC_DVI,
	OPCODE_BASIC_MOD,
	OPCODE_BASIC_MDI,
	OPCODE_BASIC_AND,
	OPCODE_BASIC_BOR,
	OPCODE_BASIC_XOR,
	OPCODE_BASIC_SHR,
	OPCODE_BASIC_ASR,
	OPCODE_BASIC_SHL,
	OPCODE_BASIC_IFB,
	OPCODE_BASIC_IFC,
	OPCODE_BASIC_IFE,
	OPCODE_BASIC_IFN,
	OPCODE_BASIC_IFG,
	OPCODE_BASIC_IFA,
	OPCODE_BASIC_IFL,
	OPCODE_BASIC_IFU,
	OPCODE_BASIC_ADX,
	OPCODE_BASIC_SBX,
	OPCODE_BASIC_STI,
	OPCODE_BASIC_STD,
};

enum {
	OPCODE_SPECIAL_RESERVED = 0,
	OPCODE_SPECIAL_JSR,
	OPCODE_SPECIAL_INT,
	OPCODE_SPECIAL_IAG,
	OPCODE_SPECIAL_IAS,
	OPCODE_SPECIAL_RFI,
	OPCODE_SPECIAL_IAQ,
	OPCODE_SPECIAL_HWN,
	OPCODE_SPECIAL_HWQ,
	OPCODE_SPECIAL_HWI,
};

enum {
	OPERATOR_PEEK = 0,
	OPERATOR_PICK,
	OPERATOR_POP,
	OPERATOR_PUSH,
};

enum {
	REGISTER_A = 0,
	REGISTER_B,
	REGISTER_C,
	REGISTER_X,
	REGISTER_Y,
	REGISTER_Z,
	REGISTER_I,
	REGISTER_J,
};

enum {
	SYMBOL_ADD_NEXT = 0,
	SYMBOL_CLOSE_BRACE,
	SYMBOL_CLOSE_PARENTHESIS,
	SYMBOL_LABEL_DELIMITOR,
	SYMBOL_LIST_SEPERATOR,
	SYMBOL_LITERAL_VALUE_DELIMITOR,
	SYMBOL_OPEN_BRACE,
	SYMBOL_OPEN_PARENTHESIS,
};

enum {
	SYSTEM_REGISTER_EX = 0,
	SYSTEM_REGISTER_IA,
	SYSTEM_REGISTER_PC,
	SYSTEM_REGISTER_SP,
};

enum {
	TOKEN_BASIC_OPCODE = 0,
	TOKEN_BEGIN,
	TOKEN_DIRECTIVE,
	TOKEN_END,
	TOKEN_LABEL,
	TOKEN_OPERAND,
	TOKEN_OPERATOR,
	TOKEN_REGISTER,
	TOKEN_SPECIAL_OPCODE,
	TOKEN_SYMBOL,
	TOKEN_SYSTEM_REGISTER,
	TOKEN_VALUE,
	TOKEN_VALUE_LIST,
};

enum {
	VALUE_HEXIDECIMAL = 0,
	VALUE_INTEGER,
	VALUE_STRING_VAR,
};

static const std::string ADDRESSING_MODE_STR[] = {
	"REGISTER", "INDIRECT REGISTER", "INDIRECT REGISTER NEXT WORD", "PUSH POP", "PEEK", "PICK", "SP LITERAL", "PC LITERAL", 
	"EX LITERAL", "INDIRECT_NEXT_WORD", "NEXT_WORD_LITERAL", "LITERAL",
};

static const word_t ADDRESSING_MODE_VAL[] = {
	0x0, 0x8, 0x10, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 
	0x1d, 0x1e, 0x1f, 0x20,
};

static const std::string DIRECTIVE_STR[] = {
	"DAT", "INCBIN", "INCLUDE", "RESERVE",
};

static const std::string OPCODE_BASIC_STR[] = {
	"SET", "ADD", "SUB", "MUL", "MLI", "DIV", "DVI", "MOD", 
	"MDI", "AND", "BOR", "XOR", "SHR", "ASR", "SHL", "IFB", 
	"IFC", "IFE", "IFN", "IFG", "IFA", "IFL", "IFU", "ADX", 
	"SBX", "STI", "STD",
};

static const word_t OPCODE_BASIC_VAL[] = {
	0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 
	0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x1a,
	0x1b, 0x1e, 0x1f,
};

static const std::string OPCODE_SPECIAL_STR[] = {
	"JSR", "INT", "IAG", "IAS", "RFI", "IAQ", "HWN", "HWQ", 
	"HWI",
};

static const word_t OPCODE_SPECIAL_VAL[] = {
	0x1, 0x8, 0x9, 0xa, 0xb, 0xc, 0x10, 0x11, 
	0x12,
};

static const std::string OPERATOR_STR[] = {
	"PEEK", "PICK", "POP", "PUSH",
};

static const std::string REGISTER_STR[] = {
	"A", "B", "C", "X", "Y", "Z", "I", "J",
};

static const char SYMBOL_CH[] = {
	'+', ']', ')', ':', ',', '#', '[', '(', 
};

static const std::string SYMBOL_STR[] = {
	"+", "]", ")", ":", ",", "#", "[", "(", 
};

static const std::string SYSTEM_REGISTER_STR[] = {
	"EX", "IA", "PC", "SP",
};

static const std::string TOKEN_STR[] = {
	"BASIC OPCODE", "BEGIN", "DIRECTIVE", "END", "LABEL", "OPERAND", "OPERATOR", "REGISTER", 
	"SPECIAL OPCODE", "SYMBOL", "SYSTEM REGISTER", "VALUE", "VALUE LIST",
};

static const std::string VALUE_STR[] = {
	"HEXIDECIMAL", "INTEGER", "STRING",
};

#define MAX_LITERAL ((word_t) 0x1e)
#define MAX_ADDRESSING_MODE ADDRESSING_MODE_LITERAL
#define MAX_DIRECTIVE DIRECTIVE_RESERVE
#define MAX_OPCODE_BASIC OPCODE_BASIC_STD
#define MAX_OPCODE_SPECIAL OPCODE_SPECIAL_HWI
#define MAX_OPERATOR OPERATOR_PUSH
#define MAX_REGISTER REGISTER_J
#define MAX_SYMBOL SYMBOL_OPEN_PARENTHESIS
#define MAX_SYSTEM_REGISTER SYSTEM_REGISTER_SP
#define MAX_TOKEN TOKEN_VALUE_LIST
#define MAX_VALUE VALUE_STRING_VAR
#define MAX_WORD ((word_t) 0xffff)
#define MIN_LITERAL INVALID_WORD
#define OPCODE_BASIC_OPERAND_COUNT (2)
#define OPCODE_SPECIAL_OPERAND_COUNT (1)
#define REGISTER_COUNT (8)
#define SYSTEM_REGISTER_COUNT (4)

static const size_t TOKEN_TYPE_LEN[] = {
	MAX_OPCODE_BASIC + 1, 0, MAX_DIRECTIVE + 1, 0, 0, 0, MAX_OPERATOR + 1, MAX_REGISTER + 1, 
	MAX_OPCODE_SPECIAL + 1, MAX_SYMBOL + 1, MAX_SYSTEM_REGISTER + 1, MAX_VALUE + 1, 0,
};

static const std::string *TOKEN_TYPE_STR[] = {
	OPCODE_BASIC_STR, NULL, DIRECTIVE_STR, NULL, NULL, NULL, OPERATOR_STR, REGISTER_STR, 
	OPCODE_SPECIAL_STR, SYMBOL_STR, SYSTEM_REGISTER_STR, VALUE_STR, NULL,
};

static const std::set<std::string> DIRECTIVE_SET(
	DIRECTIVE_STR,
	DIRECTIVE_STR + MAX_DIRECTIVE + 1
	);

static const std::set<std::string> OPCODE_BASIC_SET(
	OPCODE_BASIC_STR,
	OPCODE_BASIC_STR + MAX_OPCODE_BASIC + 1
	);

static const std::set<std::string> OPCODE_SPECIAL_SET(
	OPCODE_SPECIAL_STR,
	OPCODE_SPECIAL_STR + MAX_OPCODE_SPECIAL + 1
	);

static const std::set<std::string> OPERATOR_SET(
	OPERATOR_STR,
	OPERATOR_STR + MAX_OPERATOR + 1
	);

static const std::set<std::string> REGISTER_SET(
	REGISTER_STR,
	REGISTER_STR + MAX_REGISTER + 1
	);

static const std::set<char> SYMBOL_CH_SET(
	SYMBOL_CH,
	SYMBOL_CH + MAX_SYMBOL + 1
	);

static const std::set<std::string> SYMBOL_STR_SET(
	SYMBOL_STR,
	SYMBOL_STR + MAX_SYMBOL + 1
	);

static const std::set<std::string> SYSTEM_REGISTER_SET(
	SYSTEM_REGISTER_STR,
	SYSTEM_REGISTER_STR + MAX_SYSTEM_REGISTER + 1
	);

#define TO_UPPERCASE(_S_) TRANSFORM(::toupper, _S_)
#define ADDRESSING_MODE_STRING(_T_) (_T_ > MAX_ADDRESSING_MODE ? UNKNOWN : ADDRESSING_MODE_STR[_T_])
#define ADDRESSING_MODE_VALUE(_T_) (_T_ > MAX_ADDRESSING_MODE ? INVALID_WORD : ADDRESSING_MODE_VAL[_T_])
#define DEFINE_OPCODE_BASIC(_OP_, _A_, _B_)\
	((word_t) (((word_t) _A_ & 0x3f) << 0xa) | (((word_t) _B_ & 0x1f) << 0x5) | ((word_t) _OP_ & 0x1f))
#define DEFINE_OPCODE_SPECIAL(_OP_, _A_)\
	((word_t) (((word_t) _A_ & 0x3f) << 0xa) | (((word_t) _OP_ & 0x1f) << 0x5))
#define IS_DIRECTIVE(_S_) (DIRECTIVE_SET.find(_S_) != DIRECTIVE_SET.end())
#define IS_OPCODE_BASIC(_S_) (OPCODE_BASIC_SET.find(_S_) != OPCODE_BASIC_SET.end())
#define IS_OPCODE_SPECIAL(_S_) (OPCODE_SPECIAL_SET.find(_S_) != OPCODE_SPECIAL_SET.end())
#define IS_OPERATOR(_S_) (OPERATOR_SET.find(_S_) != OPERATOR_SET.end())
#define IS_REGISTER(_S_) (REGISTER_SET.find(_S_) != REGISTER_SET.end())
#define IS_SYMBOL_CHARACTER(_C_) (SYMBOL_CH_SET.find(_C_) != SYMBOL_CH_SET.end())
#define IS_SYMBOL_TYPE(_S_) (SYMBOL_STR_SET.find(_S_) != SYMBOL_STR_SET.end())
#define IS_SYSTEM_REGISTER(_S_) (SYSTEM_REGISTER_SET.find(_S_) != SYSTEM_REGISTER_SET.end())
#define DIRECTIVE_STRING(_T_) (_T_ > MAX_DIRECTIVE ? UNKNOWN : DIRECTIVE_STR[_T_])
#define OPCODE_BASIC_STRING(_T_) (_T_ > MAX_OPCODE_BASIC ? UNKNOWN : OPCODE_BASIC_STR[_T_])
#define OPCODE_BASIC_VALUE(_T_) (_T_ > MAX_OPCODE_BASIC ? INVALID_WORD : OPCODE_BASIC_VAL[_T_])
#define OPCODE_SPECIAL_STRING(_T_) (_T_ > MAX_OPCODE_SPECIAL ? UNKNOWN : OPCODE_SPECIAL_STR[_T_])
#define OPCODE_SPECIAL_VALUE(_T_) (_T_ > MAX_OPCODE_SPECIAL ? INVALID_WORD : OPCODE_SPECIAL_VAL[_T_])
#define OPERATOR_STRING(_T_) (_T_ > MAX_OPERATOR ? UNKNOWN : OPERATOR_STR[_T_])
#define REGISTER_STRING(_T_) (_T_ > MAX_REGISTER ? UNKNOWN : REGISTER_STR[_T_])
#define SYMBOL_STRING(_T_) (_T_ > MAX_SYMBOL ? UNKNOWN : SYMBOL_STR[_T_])
#define SYSTEM_REGISTER_STRING(_T_) (_T_ > MAX_SYSTEM_REGISTER ? UNKNOWN : SYSTEM_REGISTER_STR[_T_])
#define TOKEN_STRING(_T_) (_T_ > MAX_TOKEN ? UNKNOWN : TOKEN_STR[_T_])
#define TOKEN_TYPE_LENGTH(_T_) (_T_ > MAX_TOKEN ? 0 : TOKEN_TYPE_LEN[_T_])
#define TOKEN_TYPE_STRING(_T_) (_T_ > MAX_TOKEN ? NULL : TOKEN_TYPE_STR[_T_])
#define VALUE_STRING(_T_) (_T_ > MAX_VALUE ? UNKNOWN : VALUE_STR[_T_])

size_t __determine_token_subtype(
	const std::string &text,
	size_t type
	);

#endif