/*
 * parser.h
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

#ifndef PARSER_H_
#define PARSER_H_

#include "lexer.h"
#include "node.h"
#include "type.h"

typedef class _parser :
		public lexer {

	public:
		
		_parser(void);

		_parser(
			const std::string &input,
			bool is_file
			);

		_parser(
			const _parser &other
			);

		virtual ~_parser(void);

		_parser &operator=(
			const _parser &other
			);

		virtual void clear(void);

		virtual void discover(void);

		std::vector<std::vector<node>> export_statements(void);

		std::vector<node> &get_statement(void);

		std::vector<node> &get_statement(
			size_t position
			);

		size_t get_statement_position(void);

		bool has_next_statement(void);

		bool has_previous_statement(void);

		void import_statements(
			std::vector<std::vector<node>> statements
			);

		virtual void initialize(
			const std::string &input,
			bool is_file
			);

		virtual void initialize(
			const _parser &other
			);

		std::vector<node> &move_next_statement(void);

		std::vector<node> &move_previous_statement(void);

		void remove_statement(void);

		void remove_statement(
			size_t position
			);

		virtual void reset(void);

		virtual size_t size(void);

		std::string statement_to_string(
			bool verbose
			);

		std::string statement_to_string(
			size_t position,
			bool verbose
			);

		virtual std::string to_string(
			bool verbose
			);

	protected:

		void _advance_token(
			bool expect_eos = false
			);

		size_t _append_token(
			std::vector<node> &statement,
			size_t parent_position = INVALID_TYPE
			);

		size_t _append_token(
			const uuidl_t &id,
			std::vector<node> &statement,
			size_t parent_position = INVALID_TYPE
			);

		void _enumerate_command(
			std::vector<node> &statement
			);

		void _enumerate_directive(
			std::vector<node> &statement
			);

		void _enumerate_label(
			std::vector<node> &statement
			);

		void _enumerate_operand(
			std::vector<node> &statement,
			size_t parent_position
			);

		void _enumerate_operand_a(
			std::vector<node> &statement,
			size_t parent_position
			);

		void _enumerate_operand_b(
			std::vector<node> &statement,
			size_t parent_position
			);

		void _enumerate_statement(
			std::vector<node> &statement
			);

		void _enumerate_static_value(
			std::vector<node> &statement,
			size_t parent_position
			);

		void _enumerate_value(
			std::vector<node> &statement,
			size_t parent_position
			);

		void _enumerate_value_list(
			std::vector<node> &statement,
			size_t parent_position
			);

		void _set_operand_mode(
			std::vector<node> &statement,
			size_t operand_position,
			size_t mode
			);

		size_t _position;

		std::vector<std::vector<node>> _statement;

	private:

		std::recursive_mutex _parser_lock;

} parser, *parser_ptr;

#endif