/*
 * uuid.cpp
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

#include <ctime>
#include "type.h"
#include "uuid.h"
#include "uuid_generator.h"
#include "uuid_type.h"
#include "version.h"

_uuidl_t::_uuidl_t(void) :
	block0(0),
	block1(0)
{
	return;
}

_uuidl_t::_uuidl_t(
	const _uuidl_t &other
	) :
		block0(other.block0),
		block1(other.block1)
{
	return;
}

_uuidl_t::~_uuidl_t(void)
{
	return;
}

_uuidl_t &
_uuidl_t::operator=(
	const _uuidl_t &other
	)
{
	if(this != &other) {
		block0 = other.block0;
		block1 = other.block1;
	}

	return *this;
}

bool 
_uuidl_t::operator==(
	const _uuidl_t &other
	)
{
	return block0 == other.block0
			&& block1 == other.block1;
}

bool 
operator<(
	const _uuidl_t &left,
	const _uuidl_t &right
	)
{
	return (left.block0 > right.block0)
			|| ((left.block0 == right.block0)
					&& (left.block1 > right.block1));
}

namespace uuid {

static uuid_generator __uuid_generator;
static bool __uuid_initialize = false;
static std::recursive_mutex __uuid_lock;

uuidl_t 
generate_long_id(
	bool register_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, __uuid_lock);

	uuidl_t result;

	if(!__uuid_initialize) {
		THROW_UUID_EXCEPTION(UUID_EXCEPTION_LIB_UNINITIALIZED);
	}
	ZERO_LONG_ID(result);
	result = __uuid_generator.generate_long_id(register_id);

	return result;
}

uuids_t 
generate_short_id(
	bool register_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, __uuid_lock);

	uuids_t result;
	
	if(!__uuid_initialize) {
		THROW_UUID_EXCEPTION(UUID_EXCEPTION_LIB_UNINITIALIZED);
	}
	ZERO_SHORT_ID(result);
	result = __uuid_generator.generate_short_id(register_id);

	return result;
}

void 
initialize(void)
{
	LOCK_OBJECT(std::recursive_mutex, __uuid_lock);
	
	std::srand((uint32_t) std::time(NULL));
	__uuid_generator.initialize(rand());
	__uuid_initialize = true;
}

bool 
is_unique_long_id(
	const uuidl_t &long_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, __uuid_lock);

	if(!__uuid_initialize) {
		THROW_UUID_EXCEPTION(UUID_EXCEPTION_LIB_UNINITIALIZED);
	}

	return __uuid_generator.is_unique_long_id(long_id);
}

bool 
is_unique_short_id(
	uuids_t short_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, __uuid_lock);

	if(!__uuid_initialize) {
		THROW_UUID_EXCEPTION(UUID_EXCEPTION_LIB_UNINITIALIZED);
	}

	return __uuid_generator.is_unique_short_id(short_id);
}

void 
register_long_id(
	const uuidl_t &long_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, __uuid_lock);

	if(!__uuid_initialize) {
		THROW_UUID_EXCEPTION(UUID_EXCEPTION_LIB_UNINITIALIZED);
	}
	__uuid_generator.register_long_id(long_id);
}

void 
register_short_id(
	uuids_t short_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, __uuid_lock);

	if(!__uuid_initialize) {
		THROW_UUID_EXCEPTION(UUID_EXCEPTION_LIB_UNINITIALIZED);
	}
	__uuid_generator.register_short_id(short_id);
}

void 
unregister_long_id(
	const uuidl_t &long_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, __uuid_lock);

	if(!__uuid_initialize) {
		THROW_UUID_EXCEPTION(UUID_EXCEPTION_LIB_UNINITIALIZED);
	}
	__uuid_generator.unregister_long_id(long_id);
}

void 
unregister_short_id(
	uuids_t short_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, __uuid_lock);

	if(!__uuid_initialize) {
		THROW_UUID_EXCEPTION(UUID_EXCEPTION_LIB_UNINITIALIZED);
	}
	__uuid_generator.unregister_short_id(short_id);
}

std::string 
version(
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, __uuid_lock);

	std::stringstream ss;

	if(verbose) {
		ss << VERSION_STRING_VERBOSE;
	} else {
		ss << VERSION_STRING;
	}

	return ss.str();
}
}