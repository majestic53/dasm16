/*
 * lexer.h
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

#ifndef LEXER_H_
#define LEXER_H_

#include <vector>
#include "lexer_base.h"
#include "token.h"
#include "uuid.h"

typedef class _lexer :
		public lexer_base{

	public:

		_lexer(void);

		_lexer(
			const std::string &input,
			bool is_file
			);

		_lexer(
			const _lexer &other
			);

		virtual ~_lexer(void);

		_lexer &operator=(
			const _lexer &other
			);

		virtual void clear(void);

		virtual void discover(void);

		uuidl_t get_begin_token_id(void);

		uuidl_t get_end_token_id(void);

		token &get_token(void);

		token &get_token(
			uuidl_t id
			);

		size_t get_token_position(void);

		bool has_token(
			uuidl_t id
			);

		bool has_next_token(void);

		bool has_previous_token(void);

		virtual void initialize(
			const std::string &input,
			bool is_file
			);

		virtual void initialize(
			const _lexer &other
			);

		uuidl_t insert_token(
			token &tok
			);

		uuidl_t insert_token(
			token &tok,
			size_t position
			);

		token &move_next_token(void);

		token &move_previous_token(void);

		virtual void reset(void);

		virtual size_t size(void);

		virtual std::string to_string(
			bool verbose
			);

	protected:

		void _advance_character(
			bool expect_eos = false
			);

		void _enumerate_alpha_token(
			token &tok
			);

		void _enumerate_digit_token(
			token &tok
			);

		void _enumerate_symbol_token(
			token &tok
			);

		void _skip_whitespace(void);

		size_t _position;

		std::map<uuidl_t, token> _token;

		std::vector<uuidl_t> _token_position;

	private:

		std::recursive_mutex _lexer_lock;

} lexer, *lexer_ptr;

#endif