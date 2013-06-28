/*
 * uuid_generator.cpp
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

#include "uuid_generator.h"
#include "uuid_generator_type.h"

_uuid_generator::_uuid_generator(void)
{
	clear();
}

_uuid_generator::_uuid_generator(
	const _uuid_generator &other
	)
{
	initialize(other);
}

_uuid_generator::~_uuid_generator(void)
{
	return;
}

_uuid_generator &
_uuid_generator::operator=(
	const _uuid_generator &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);

	if(this != &other) {
		initialize(other);
	}

	return *this;
}

uint32_t 
_uuid_generator::_generate_value(void)
{
	uint32_t result;

	if(!_mt_value_position) {
		_regenerate_values();
	}
	result = _mt_value.at(_mt_value_position);
	_mt_value_position %= MT_VALUE_LENGTH;
	result ^= (result >> 0xb);
	result ^= ((result << 0x7) & 0x9d2c5680);
	result ^= ((result << 0xf) & 0xefc60000);
	result ^= (result >> 0x12);

	return result;
}
		
void 
_uuid_generator::_initialize(
	uint32_t seed
	)
{
	size_t i = 1;

	clear();
	_mt_seed = seed;
	_mt_value.resize(MT_VALUE_LENGTH);
	_mt_value.at(0) = seed;

	for(; i < MT_VALUE_LENGTH; ++i) {
		_mt_value.at(i) = (uint32_t) (0x6c078965 * (_mt_value.at(i - 1) ^ (_mt_value.at(i - 1) >> 0x1e)) + i);
	}
}

void 
_uuid_generator::_regenerate_values(void)
{
	size_t i = 0;
	uint32_t value;

	for(; i < MT_VALUE_LENGTH; ++i) {
		value = (_mt_value.at(i) & 0x80000000) + (_mt_value.at((i + 1) % MT_VALUE_LENGTH) & 0x7fffffff);
		_mt_value.at(i) = _mt_value.at((i + 0x18d) % MT_VALUE_LENGTH) ^ (value >> 1);

		if(value % 2) {
			_mt_value.at(i) ^= 0x9908b0df;
		}
	}
}

void 
_uuid_generator::clear(void)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);

	_mt_seed = 0;
	_mt_value.clear();
	_mt_value_position = 0;
	_registered_long_id.clear();
	_registered_short_id.clear();
}

uuidl_t 
_uuid_generator::generate_long_id(
	bool register_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);

	uuidl_t result;

	result.block0 = (((uint64_t) _generate_value() << 0x20)) | _generate_value();
	result.block1 = (((uint64_t) _generate_value() << 0x20)) | _generate_value();

	if(register_id) {
		register_long_id(result);
	}

	return result;
}

uuids_t 
_uuid_generator::generate_short_id(
	bool register_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);

	uuids_t result = (((uint64_t) _generate_value() << 0x20)) | _generate_value();

	if(register_id) {
		register_short_id(result);
	}

	return result;
}

void 
_uuid_generator::initialize(
	uint32_t seed
	)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);

	_initialize(seed);
}

void 
_uuid_generator::initialize(
	const _uuid_generator &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);

	_initialize(other._mt_seed);
}

bool 
_uuid_generator::is_unique_long_id(
	const uuidl_t &long_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);
	
	return _registered_long_id.find(long_id) == _registered_long_id.end();
}

bool 
_uuid_generator::is_unique_short_id(
	uuids_t short_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);

	return _registered_short_id.find(short_id) == _registered_short_id.end();
}

void 
_uuid_generator::register_long_id(
	const uuidl_t &long_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);

	if(_registered_long_id.find(long_id) != _registered_long_id.end()){ 
		THROW_UUID_GENERATOR_EXCEPTION_WITH_MESSAGE(
			UUID_GENERATOR_EXCEPTION_ID_ALREADY_REGISTERED,
			"{" << LONG_ID_AS_STRING(long_id) << "}"
			);
	}
	_registered_long_id.insert(long_id);
}

void 
_uuid_generator::register_short_id(
	uuids_t short_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);

	if(_registered_short_id.find(short_id) != _registered_short_id.end()){ 
		THROW_UUID_GENERATOR_EXCEPTION_WITH_MESSAGE(
			UUID_GENERATOR_EXCEPTION_ID_ALREADY_REGISTERED,
			"{" << SHORT_ID_AS_STRING(short_id) << "}"
			);
	}
	_registered_short_id.insert(short_id);
}

void 
_uuid_generator::unregister_long_id(
	const uuidl_t &long_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);

	std::set<uuidl_t>::iterator long_id_iter = _registered_long_id.find(long_id);

	if(long_id_iter == _registered_long_id.end()) {
		THROW_UUID_GENERATOR_EXCEPTION_WITH_MESSAGE(
			UUID_GENERATOR_EXCEPTION_ID_NOT_REGISTERED,
			"{" << LONG_ID_AS_STRING(long_id) << "}"
			);
	}
	_registered_long_id.erase(long_id_iter);
}

void 
_uuid_generator::unregister_short_id(
	uuids_t short_id
	)
{
	LOCK_OBJECT(std::recursive_mutex, _uuid_generator_lock);

	std::set<uuids_t>::iterator short_id_iter = _registered_short_id.find(short_id);

	if(short_id_iter == _registered_short_id.end()) {
		THROW_UUID_GENERATOR_EXCEPTION_WITH_MESSAGE(
			UUID_GENERATOR_EXCEPTION_ID_NOT_REGISTERED,
			"{" << SHORT_ID_AS_STRING(short_id) << "}"
			);
	}
	_registered_short_id.erase(short_id_iter);
}