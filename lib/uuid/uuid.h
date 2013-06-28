/*
 * uuid.h
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

#ifndef UUID_H_
#define UUID_H_

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

#define IS_LONG_ID_EQUAL(_UL0_, _UL1_)\
	(_UL0_.block0 == _UL1_.block0 && _UL0_.block1 == _UL1_.block1)
#define IS_SHORT_ID_EQUAL(_US0_, _US1_) (_US0_ == _US1_)
#define VALUE_AS_HEX(_T_, _V_)\
	std::setw(sizeof(_T_) * 2) << std::setfill('0') << std::hex << ((unsigned) _V_) << std::dec
#define SHORT_ID_AS_STRING(_UL_)\
	VALUE_AS_HEX(uint32_t, (uint32_t) (_UL_ >> 0x20)) << "-"\
	<< VALUE_AS_HEX(uint16_t, (uint16_t) (_UL_ >> 0x10)) << "-"\
	<< VALUE_AS_HEX(uint16_t, (uint16_t) _UL_)
#define LONG_ID_AS_STRING(_US_)\
	SHORT_ID_AS_STRING(_US_.block0) << "-"\
	<< VALUE_AS_HEX(uint16_t, (uint16_t) (_US_.block1 >> 0x30)) << "-"\
	<< VALUE_AS_HEX(uint16_t, (uint16_t) (_US_.block1 >> 0x20)) << "-"\
	<< VALUE_AS_HEX(uint32_t, (uint32_t) _US_.block1)
#define ZERO_LONG_ID(_UL_) _UL_.block0 = 0; _UL_.block1 = 0;
#define ZERO_SHORT_ID(_US_) _US_ = 0;

typedef uint64_t uuids_t, *uuids_ptr;

typedef class _uuidl_t {

	public:

		_uuidl_t(void);

		_uuidl_t(
			const _uuidl_t &other
			);

		virtual ~_uuidl_t(void);

		_uuidl_t &operator=(
			const _uuidl_t &other
			);

		bool operator==(
			const _uuidl_t &other
			);

		friend bool operator<(
			const _uuidl_t &left,
			const _uuidl_t &right
			);

		uuids_t block0;

		uuids_t block1;	

} uuidl_t, *uuidl_ptr;

namespace uuid {

	static uuidl_t NULL_LONG_ID;
	static uuids_t NULL_SHORT_ID = 0;

	uuidl_t generate_long_id(
		bool register_id = true
		);

	uuidl_t generate_long_id(
		bool register_id
		);

	uuids_t generate_short_id(
		bool register_id = true
		);

	uuids_t generate_short_id(
		bool register_id
		);

	void initialize(void);

	bool is_unique_long_id(
		const uuidl_t &long_id
		);

	bool is_unique_short_id(
		uuids_t short_id
		);

	void register_long_id(
		const uuidl_t &long_id
		);

	void register_short_id(
		uuids_t short_id
		);

	void unregister_long_id(
		const uuidl_t &long_id
		);

	void unregister_short_id(
		uuids_t short_id
		);

	std::string version(
		bool verbose
		);
}

#endif