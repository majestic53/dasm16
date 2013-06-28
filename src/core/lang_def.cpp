/*
 * lang_def.cpp
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

#include "lang_def.h"

size_t 
__determine_token_subtype(
	const std::string &text,
	size_t type
	)
{
	std::string *subtypes = NULL;
	size_t i = 0, result = TOKEN_TYPE_LENGTH(type);

	if(result > 0) {
		subtypes = (std::string *) TOKEN_TYPE_STRING(type);

		if(subtypes) {

			for(; i < result; ++i) {

				if(text == subtypes[i]) {
					result = i;
					break;
				}
			}
		} else {
			result = INVALID_TYPE;
		}
	} else {
		result = INVALID_TYPE;
	}

	return result;
}