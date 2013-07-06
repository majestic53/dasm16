/*
 * assembler.h
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

#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include "parser.h"
#include "type.h"

typedef class _assembler :
		public parser {

	public:

		_assembler(void);

		_assembler(
			const std::string &input,
			bool is_file
			);

		_assembler(
			const _assembler &other
			);

		virtual ~_assembler(void);

		_assembler &operator=(
			const _assembler &other
			);

		virtual void clear(void);

		virtual void generate(
			const std::string &output,
			bool verbose
			);

		std::vector<word_t> &get_binary_data(void);

		bool has_binary_data(void);

		virtual void initialize(
			const std::string &input,
			bool is_file
			);

		virtual void initialize(
			const _assembler &other
			);

		virtual size_t size(void);

		virtual std::string to_string(
			bool verbose
			);

	protected:

		std::vector<word_t> &_evaluate_operand(
			std::vector<node> &statement,
			size_t position
			);

		std::vector<word_t> &_evaluate_value(
			token &value_token,
			bool literal_value = false
			);

		void _generate_pass_0(void);

		void _generate_pass_1(void);

		void _write_file(
			const std::string &output
			);

		std::vector<word_t> _binary_file;

		std::map<uuidl_t, std::vector<word_t>> _binary_include;

		std::map<std::string, word_t> _label_offset;

		std::map<uuidl_t, std::vector<word_t>> _value;

	private:

		std::recursive_mutex _assembler_lock;

} assembler, *assembler_ptr;

#endif