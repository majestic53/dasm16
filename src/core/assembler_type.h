/*
 * assembler_type.h
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

#ifndef ASSEMBLER_TYPE_H_
#define ASSEMBLER_TYPE_H_

#include "type.h"

enum {
	ASSEMBLER_EXCEPTION_BINARY_FILE_TOO_LARGE = 0,
	ASSEMBLER_EXCEPTION_BINARY_FILE_UNALIGNED,
	ASSEMBLER_EXCEPTION_EXPECTED_OPERAND_NODE,
	ASSEMBLER_EXCEPTION_EXPECTED_VALUE_NODE,
	ASSEMBLER_EXCEPTION_INCLUDE_FILE_NOT_FOUND,
	ASSEMBLER_EXCEPTION_INVALID_STATEMENT_POSITION,
	ASSEMBLER_EXCEPTION_LABEL_REDEFINITION,
	ASSEMBLER_EXCEPTION_LABEL_UNDEFINED,
	ASSEMBLER_EXCEPTION_LITERAL_VALUE_OUT_OR_RANGE,
	ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
	ASSEMBLER_EXCEPTION_RESERVATION_TOO_LARGE,
	ASSEMBLER_EXCEPTION_VALUE_OUT_OR_RANGE,
	ASSEMBLER_EXCEPTION_WRITE_FILE_FAILED,
};

static const std::string ASSEMBLER_EXCEPTION_STR[] = {
	"Binary file exceeds maximum length",
	"Binary file unaligned",
	"Expected operand node",
	"Expected value node",
	"Include file not found",
	"Invalid statement position",
	"Label redifinition",
	"Label undefined",
	"Literal value out of range",
	"Malformed statement",
	"Reservation exceeds maximum length",
	"Value out of range",
	"Failed to write to file",
};

#define ASSEMBLER_EXCPETION_HEADER "ASSEMBLER"
#define ASSEMBLER_RESERVED_VALUE (0)
#define BINARY_BLOCK_WIDTH (8)
#define BINARY_FILE_EXTENSION ".bin"
#define HEXIDECIMAL_RADIX (16)
#define HEXIDECIMAL_OFFSET (10)
#define INTEGER_RADIX (10)
#define MAX_ASSEMBLER_EXCEPTION ASSEMBLER_EXCEPTION_WRITE_FILE_FAILED
#define SHOW_ASSEMBLER_EXCEPTION_HEADER true

#define ASSEMBLER_EXCEPTION_STRING(_T_)\
	(_T_ > MAX_ASSEMBLER_EXCEPTION ? UNKNOWN_EXCEPTION : ASSEMBLER_EXCEPTION_STR[_T_])
#define THROW_ASSEMBLER_EXCEPTION(_T_)\
	THROW_EXCEPTION(ASSEMBLER_EXCPETION_HEADER, ASSEMBLER_EXCEPTION_STRING(_T_), SHOW_ASSEMBLER_EXCEPTION_HEADER)
#define THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(_T_, _M_)\
	THROW_EXCEPTION_WITH_MESSAGE(ASSEMBLER_EXCPETION_HEADER, ASSEMBLER_EXCEPTION_STRING(_T_), _M_, SHOW_ASSEMBLER_EXCEPTION_HEADER)

class _assembler;
typedef _assembler assembler, *assembler_ptr;

#endif