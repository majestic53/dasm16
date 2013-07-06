/*
 * lexer_base.h
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

#ifndef LEXER_BASE_H_
#define LEXER_BASE_H_

#include <map>
#include "type.h"

typedef class _lexer_base {

	public:

		_lexer_base(void);

		_lexer_base(
			const std::string &input,
			bool is_file
			);

		_lexer_base(
			const _lexer_base &other
			);

		virtual ~_lexer_base(void);

		_lexer_base &operator=(
			const _lexer_base &other
			);

		virtual void clear(void);

		char get_character(void);

		size_t get_character_column(void);

		size_t get_character_position(void);

		size_t get_character_row(void);

		size_t get_character_type(void);

		std::string get_origin(void);

		std::string get_origin_path(void);

		bool has_next_character(void);

		bool has_previous_character(void);

		virtual void initialize(
			const std::string &input,
			bool is_file
			);

		virtual void initialize(
			const _lexer_base &other
			);

		char move_next_character(void);

		char move_previous_character(void);

		virtual void reset(void);

		virtual size_t size(void);

		virtual std::string to_string(
			bool verbose
			);

	protected:

		void _determine_character_type(void);

		size_t _column;

		std::map<size_t, size_t> _column_length;

		std::string _input;

		std::string _origin;

		size_t _position;

		size_t _row;

		size_t _type;

	private:

		std::recursive_mutex _lexer_base_lock;

} lexer_base, *lexer_base_ptr;

#endif