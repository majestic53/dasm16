/*
 * lexer_type.h
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

#ifndef LEXER_TYPE_H_
#define LEXER_TYPE_H_

#include "type.h"

enum {
	LEXER_EXCEPTION_EXPECTED_ALPHA_CHARACTER = 0,
	LEXER_EXCEPTION_EXPECTED_DIGIT_CHARACTER,
	LEXER_EXCEPTION_EXPECTED_END_OF_CHARACTER_STREAM,
	LEXER_EXCEPTION_EXPECTED_SYMBOL_CHARACTER,
	LEXER_EXCEPTION_INVALID_TOKEN_POSITION,
	LEXER_EXCEPTION_INVALID_TOKEN_TYPE,
	LEXER_EXCEPTION_NO_NEXT_TOKEN,
	LEXER_EXCEPTION_NO_PREVIOUS_TOKEN,
	LEXER_EXCEPTION_TOKEN_NOT_FOUND,
	LEXER_EXCEPTION_UNEXPECTED_END_OF_CHARACTER_STREAM,
	LEXER_EXCEPTION_UNKNOWN_SYMBOL_CHARACTER,
	LEXER_EXCEPTION_UNTERMINATED_STRING_LITERAL,
};

static const std::string LEXER_EXCEPTION_STR[] = {
	"Expected alpha character",
	"Expected digit character",
	"Expected end of character stream",
	"Expected symbol character",
	"Invalid token position",
	"Invalid token type",
	"No next token",
	"No previous token",
	"Token not found",
	"Unexpected end of character stream",
	"Unknown symbol character",
	"Unterminated string literal",
};

#define CHARACTER_COMMENT ';'
#define CHARACTER_HEXIDECIMAL_DELIMITOR_LOWER 'x'
#define CHARACTER_HEXIDECIMAL_DELIMITOR_UPPER 'X'
#define CHARACTER_HEXIDECIMAL_LOWER_0 'a'
#define CHARACTER_HEXIDECIMAL_LOWER_1 'f'
#define CHARACTER_HEXIDECIMAL_UPPER_0 'A'
#define CHARACTER_HEXIDECIMAL_UPPER_1 'F'
#define CHARACTER_STRING_DELIMITOR '\"'
#define CHARACTER_UNDERSCORE '_'
#define CHARACTER_ZERO '0'
#define LEXER_EXCEPTION_HEADER "LEXER"
#define MAX_LEXER_EXCEPTION LEXER_EXCEPTION_UNTERMINATED_STRING_LITERAL
#define SHOW_LEXER_EXCEPTION_HEADER true

#define IS_HEXIDECIMAL_CHARACTER(_C_)\
	((_C_ >= CHARACTER_HEXIDECIMAL_LOWER_0 && _C_ <= CHARACTER_HEXIDECIMAL_LOWER_1)\
	|| (_C_ >= CHARACTER_HEXIDECIMAL_UPPER_0 && _C_ <= CHARACTER_HEXIDECIMAL_UPPER_1))
#define IS_VALUE_TYPE(_C_) (isxdigit(_C_) != 0)
#define LEXER_EXCEPTION_STRING(_T_)\
	(_T_ > MAX_LEXER_EXCEPTION ? UNKNOWN_EXCEPTION : LEXER_EXCEPTION_STR[_T_])
#define THROW_LEXER_EXCEPTION(_T_)\
	THROW_EXCEPTION(LEXER_EXCEPTION_HEADER, LEXER_EXCEPTION_STRING(_T_), SHOW_LEXER_EXCEPTION_HEADER)
#define THROW_LEXER_EXCEPTION_WITH_MESSAGE(_T_, _M_)\
	THROW_EXCEPTION_WITH_MESSAGE(LEXER_EXCEPTION_HEADER, LEXER_EXCEPTION_STRING(_T_), _M_, SHOW_LEXER_EXCEPTION_HEADER)

class _lexer;
typedef _lexer lexer, *lexer_ptr;

#endif