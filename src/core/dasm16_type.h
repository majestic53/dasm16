/*
 * dasm16_type.h
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

#ifndef DASM16_TYPE_H_
#define DASM16_TYPE_H_

#include "type.h"

enum {
	DASM16_EXCEPTION_NOT_INITIALIZED = 0,
};

static const std::string DASM16_EXCEPTION_STR[] = {
	"Assembler is uninitialized",
};

#define DASM16_EXCEPTION_HEADER "DASM16"
#define MAX_DASM16_EXCEPTION DASM16_EXCEPTION_NOT_INITIALIZED
#define SHOW_DASM16_EXCEPTION_HEADER true

#define DASM16_EXCEPTION_STRING(_T_)\
	(_T_ > MAX_DASM16_EXCEPTION ? UNKNOWN_EXCEPTION : DASM16_EXCEPTION_STR[_T_])
#define THROW_DASM16_EXCEPTION(_T_)\
	THROW_EXCEPTION(DASM16_EXCEPTION_HEADER, DASM16_EXCEPTION_STRING(_T_), SHOW_DASM16_EXCEPTION_HEADER)
#define THROW_DASM16_EXCEPTION_WITH_MESSAGE(_T_, _M_)\
	THROW_EXCEPTION_WITH_MESSAGE(DASM16_EXCEPTION_HEADER, DASM16_EXCEPTION_STRING(_T_), _M_, SHOW_DASM16_EXCEPTION_HEADER)

#endif