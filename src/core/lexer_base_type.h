/*
 * lexer_base_type.h
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

#ifndef LEXER_BASE_TYPE_H_
#define LEXER_BASE_TYPE_H_

#include "type.h"

enum {
	CHARACTER_ALPHA = 0,
	CHARACTER_DIGIT,
	CHARACTER_SYMBOL,
	CHARACTER_WHITESPACE,
};

enum {
	LEXER_BASE_EXCEPTION_FILE_NOT_FOUND = 0,
	LEXER_BASE_EXCEPTION_INVALID_CHARACTER_POSITION,
	LEXER_BASE_EXCEPTION_INVALID_COLUMN_LENGTH,
	LEXER_BASE_EXCEPTION_NO_NEXT_CHARACTER,
	LEXER_BASE_EXCEPTION_NO_PREVIOUS_CHARACTER,
};

static const std::string CHARACTER_TYPE_STR[] = {
	"ALPHA", "DIGIT", "SYMBOL", "WHITESPACE",
};

static const std::string LEXER_BASE_EXCEPTION_STR[] = {
	"File not found",
	"Invalid character position",
	"Invalid column length",
	"No next character",
	"No previous character",
};

#define CHARACTER_DIRECTORY '\\'
#define CHARACTER_END_STREAM '\0'
#define CHARACTER_NEWLINE '\n'
#define DEFAULT_ORIGIN ".\\"
#define LEXER_BASE_EXCEPTION_HEADER "LEXER BASE"
#define MAX_CHARACTER_TYPE CHARACTER_WHITESPACE
#define MAX_LEXER_BASE_EXCEPTION LEXER_BASE_EXCEPTION_NO_PREVIOUS_CHARACTER
#define SHOW_LEXER_BASE_EXCEPTION_HEADER true

#define CHARACTER_TYPE_STRING(_T_)\
	(_T_ > MAX_CHARACTER_TYPE ? UNKNOWN : CHARACTER_TYPE_STR[_T_])
#define IS_CHARACTER_ALPHA(_C_) (isalpha(_C_) != 0)
#define IS_CHARACTER_DIGIT(_C_) (isdigit(_C_) != 0)
#define IS_CHARACTER_WHITESPACE(_C_) (isspace(_C_) != 0)
#define LEXER_BASE_EXCEPTION_STRING(_T_)\
	(_T_ > MAX_LEXER_BASE_EXCEPTION ? UNKNOWN : LEXER_BASE_EXCEPTION_STR[_T_])
#define POSITION_STRING(_P_, _R_, _C_) "pos. " << _P_ << " {" << _R_ << ", " << _C_ << "}"
#define THROW_LEXER_BASE_EXCEPTION(_T_)\
	THROW_EXCEPTION(LEXER_BASE_EXCEPTION_HEADER, LEXER_BASE_EXCEPTION_STRING(_T_), SHOW_LEXER_BASE_EXCEPTION_HEADER)
#define THROW_LEXER_BASE_EXCEPTION_WITH_MESSAGE(_T_, _M_)\
	THROW_EXCEPTION_WITH_MESSAGE(LEXER_BASE_EXCEPTION_HEADER, LEXER_BASE_EXCEPTION_STRING(_T_), _M_, SHOW_LEXER_BASE_EXCEPTION_HEADER)

class _lexer_base;
typedef _lexer_base lexer_base, *lexer_base_ptr;

#endif