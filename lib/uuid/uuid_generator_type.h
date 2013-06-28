/*
 * uuid_generator_type.h
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

#ifndef UUID_GENERATOR_TYPE_H_
#define UUID_GENERATOR_TYPE_H_

#include "type.h"

enum {
	UUID_GENERATOR_EXCEPTION_ID_ALREADY_REGISTERED = 0,
	UUID_GENERATOR_EXCEPTION_ID_NOT_REGISTERED,
};

static const std::string UUID_GENERATOR_STR[] = {
	"Identifier already registered",
	"Identifier not registered",
};

#define MAX_UUID_GENERATOR_EXCEPTION UUID_GENERATOR_EXCEPTION_ID_NOT_REGISTERED
#define MT_VALUE_LENGTH 0x270
#define SHOW_UUID_GENERATOR_HEADER true
#define UUID_GENERATOR_EXCEPTION_HEADER "UUID_GENERATOR"

#define UUID_GENERATOR_EXCEPTION_STRING(_T_)\
	(_T_ > MAX_UUID_GENERATOR_EXCEPTION ? UNKNOWN_EXCEPTION : UUID_GENERATOR_STR[_T_])
#define THROW_UUID_GENERATOR_EXCEPTION(_T_)\
	THROW_EXCEPTION(UUID_GENERATOR_EXCEPTION_HEADER, UUID_GENERATOR_EXCEPTION_STRING(_T_), SHOW_UUID_GENERATOR_HEADER)
#define THROW_UUID_GENERATOR_EXCEPTION_WITH_MESSAGE(_T_, _M_)\
	THROW_EXCEPTION_WITH_MESSAGE(UUID_GENERATOR_EXCEPTION_HEADER, UUID_GENERATOR_EXCEPTION_STRING(_T_), _M_, SHOW_UUID_GENERATOR_HEADER)

class _uuid_generator;
typedef _uuid_generator uuid_generator, *uuid_generator_ptr;

#endif