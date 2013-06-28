/*
 * dasm16.cpp
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

#include "assembler.h"
#include "dasm16.h"
#include "dasm16_type.h"
#include "type.h"
#include "uuid.h"
#include "version.h"

namespace dasm16 {

static bool __dasm_initialized = false;
static std::recursive_mutex __dasm16_lock;

void 
initialize(void)
{
	LOCK_OBJECT(std::recursive_mutex, __dasm16_lock);

	uuid::initialize();
	__dasm_initialized = true;
}

std::vector<word_t> 
invoke_assembler(
	const std::string &in_file,
	bool is_file,
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, __dasm16_lock);

	if(!__dasm_initialized) {
		THROW_DASM16_EXCEPTION(DASM16_EXCEPTION_NOT_INITIALIZED);
	}
	assembler assemb(in_file, is_file);
	assemb.generate(std::string(), verbose);

	return assemb.get_binary_data();
}

void 
invoke_assembler(
	const std::string &in_file,
	const std::string &out_file,
	bool is_file,
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, __dasm16_lock);

	if(!__dasm_initialized) {
		THROW_DASM16_EXCEPTION(DASM16_EXCEPTION_NOT_INITIALIZED);
	}
	assembler assemb(in_file, is_file);
	assemb.generate(out_file, verbose);

	if(verbose) {
		std::cout << assemb.to_string(true) << std::endl;
	}
}

std::string 
version(
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, __dasm16_lock);

	std::stringstream ss;

	if(verbose) {
		ss << VERSION_STRING_VERBOSE;
	} else {
		ss << VERSION_STRING;
	}

	return ss.str();
}
}