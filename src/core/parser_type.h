/*
 * parser_type.h
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

#ifndef PARSER_TYPE_H_
#define PARSER_TYPE_H_

#include "type.h"

enum {
	PARSER_EXCEPTION_EXPECTED_CLOSE_BRACE = 0,
	PARSER_EXCEPTION_EXPECTED_CLOSE_PARENTHESIS,
	PARSER_EXCEPTION_EXPECTED_COMMAND,
	PARSER_EXCEPTION_EXPECTED_DIRECTIVE,
	PARSER_EXCEPTION_EXPECTED_END_OF_TOKEN_STREAM,
	PARSER_EXCEPTION_EXPECTED_LABEL,
	PARSER_EXCEPTION_EXPECTED_OPEN_BRACE,
	PARSER_EXCEPTION_EXPECTED_REGISTER,
	PARSER_EXCEPTION_EXPECTED_SEPERATOR,
	PARSER_EXCEPTION_EXPECTED_STATEMENT,
	PARSER_EXCEPTION_EXPECTED_STATIC_VALUE,
	PARSER_EXCEPTION_EXPECTED_STRING,
	PARSER_EXCEPTION_EXPECTED_VALUE,
	PARSER_EXCEPTION_ILLEGAL_OPERATOR,
	PARSER_EXCEPTION_ILLEGAL_SYSTEM_REGISTER,
	PARSER_EXCEPTION_INVALID_OPERAND_POSITION,
	PARSER_EXCEPTION_INVALID_PARENT_POSITION,
	PARSER_EXCEPTION_INVALID_STATEMENT_POSITION,
	PARSER_EXCEPTION_NO_NEXT_STATEMENT,
	PARSER_EXCEPTION_NO_PREVIOUS_STATEMENT,
	PARSER_EXCEPTION_UNEXPECTED_END_OF_TOKEN_STREAM,
};

static const std::string PARSER_EXCEPTION_STR[] = {
	"Expected close brace",
	"Expected close parenthesis",
	"Expected command",
	"Expected directive",
	"Expected end of token stream",
	"Expected label",
	"Expected open brace",
	"Expected register",
	"Expected seperator",
	"Expected statement",
	"Expected static value",
	"Expected string literal",
	"Expected value",
	"Illegal operator",
	"Illegal system register",
	"Invalid operand position",
	"Invalid parent position",
	"Invalid statement position",
	"No next statement",
	"No previous statement",
	"Unexpected end of token stream",
};

#define MAX_PARSER_EXCEPTION PARSER_EXCEPTION_UNEXPECTED_END_OF_TOKEN_STREAM
#define PARSER_EXCEPTION_HEADER "PARSER"
#define SHOW_PARSER_EXCEPTION_HEADER true

#define PARSER_EXCEPTION_STRING(_T_)\
	(_T_ > MAX_PARSER_EXCEPTION ? UNKNOWN_EXCEPTION : PARSER_EXCEPTION_STR[_T_])
#define THROW_PARSER_EXCEPTION(_T_)\
	THROW_EXCEPTION(PARSER_EXCEPTION_HEADER, PARSER_EXCEPTION_STRING(_T_), SHOW_PARSER_EXCEPTION_HEADER)
#define THROW_PARSER_EXCEPTION_WITH_MESSAGE(_T_, _M_)\
	THROW_EXCEPTION_WITH_MESSAGE(PARSER_EXCEPTION_HEADER, PARSER_EXCEPTION_STRING(_T_), _M_, SHOW_PARSER_EXCEPTION_HEADER)

class _parser;
typedef _parser parser, *parser_ptr;

#endif