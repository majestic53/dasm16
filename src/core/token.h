/*
 * token.h
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

#ifndef TOKEN_H_
#define TOKEN_H_

#include "type.h"
#include "uuid.h"

typedef class _token {

	public:

		_token(void);

		_token(
			size_t type
			);

		_token(
			size_t type,
			size_t subtype
			);

		_token(
			const _token &other
			);

		virtual ~_token(void);

		_token &operator=(
			const _token &other
			);

		virtual void clear(void);

		uuidl_t get_id(void);

		size_t get_line(void);

		size_t get_mode(void);

		std::string &get_origin(void);

		size_t get_subtype(void);

		std::string &get_text(void);

		size_t get_type(void);

		bool has_mode(void);

		bool has_origin(void);

		bool has_text(void);

		void set_line(
			size_t line
			);

		void set_mode(
			size_t mode
			);

		void set_origin(
			const std::string &origin
			);

		void set_subtype(
			size_t subtype
			);

		void set_text(
			const std::string &text
			);

		void set_type(
			size_t type
			);

		virtual std::string to_string(
			bool verbose
			);

	protected:

		uuidl_t _id;

		size_t _line;

		size_t _mode;

		std::string _origin;

		size_t _subtype;

		std::string _text;

		size_t _type;

	private:

		std::recursive_mutex _token_lock;

} token, *token_ptr;

#endif