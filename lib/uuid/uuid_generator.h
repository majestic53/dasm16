/*
 * uuid_generator.h
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

#ifndef UUID_GENERATOR_H_
#define UUID_GENERATOR_H_

#include <set>
#include <vector>
#include "type.h"
#include "uuid.h"

typedef class _uuid_generator {

	public:

		_uuid_generator(void);

		_uuid_generator(
			const _uuid_generator &other
			);

		virtual ~_uuid_generator(void);

		_uuid_generator &operator=(
			const _uuid_generator &other
			);

		void clear(void);

		uuidl_t generate_long_id(
			bool register_id
			);

		uuids_t generate_short_id(
			bool register_id
			);

		void initialize(
			uint32_t seed
			);

		void initialize(
			const _uuid_generator &other
			);

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

	protected:

		uint32_t _generate_value(void);
		
		void _initialize(
			uint32_t seed
			);

		void _regenerate_values(void);

		uint32_t _mt_seed;
		
		std::vector<uint32_t> _mt_value;

		uint32_t _mt_value_position;

		std::set<uuidl_t> _registered_long_id;

		std::set<uuids_t> _registered_short_id;

	private:

		std::recursive_mutex _uuid_generator_lock;

} uuid_generator, *uuid_generator_ptr;

#endif