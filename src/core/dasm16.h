/*
 * dasm16.h
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

#ifndef DASM16_H_
#define DASM16_H_

#include <cstdint>
#include <string>
#include <vector>

typedef uint16_t word_t, *word_ptr;

namespace dasm16 {

	void initialize(void);

	std::vector<word_t> invoke_assembler(
		const std::string &in_file,
		bool is_file,
		bool verbose
		);

	void invoke_assembler(
		const std::string &in_file,
		const std::string &out_file,
		bool is_file,
		bool verbose
		);

	std::string version(
		bool verbose
		);
}

#endif