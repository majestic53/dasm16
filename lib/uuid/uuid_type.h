/*
 * uuid_type.h
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

#ifndef UUID_TYPE_H_
#define UUID_TYPE_H_

#include "type.h"

enum {
	UUID_EXCEPTION_LIB_UNINITIALIZED = 0,
};

static const std::string UUID_EXCEPTION_STR[] = {
	"Library is uninitialized",
};

#define UUID_EXCEPTION_HEADER "UUID"
#define MAX_UUID_EXCEPTION UUID_EXCEPTION_LIB_UNINITIALIZED
#define SHOW_UUID_EXCEPTION_HEADER true

#define UUID_EXCEPTION_STRING(_T_)\
	(_T_ > MAX_UUID_EXCEPTION ? UNKNOWN_EXCEPTION : UUID_EXCEPTION_STR[_T_])
#define THROW_UUID_EXCEPTION(_T_)\
	THROW_EXCEPTION(UUID_EXCEPTION_HEADER, UUID_EXCEPTION_STRING(_T_), SHOW_UUID_EXCEPTION_HEADER)
#define THROW_UUID_EXCEPTION_WITH_MESSAGE(_T_, _M_)\
	THROW_EXCEPTION_WITH_MESSAGE(UUID_EXCEPTION_HEADER, UUID_EXCEPTION_STRING(_T_), _M_, SHOW_UUID_EXCEPTION_HEADER)

#endif