/*
 * node_type.h
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

#ifndef NODE_TYPE_H_
#define NODE_TYPE_H_

#include "type.h"

enum {
	NODE_EXCEPTION_INVALID_CHILD_POSITION = 0,
};

static const std::string NODE_EXCEPTION_STR[] = {
	"Invalid child position",
};

#define MAX_NODE_EXCEPTION NODE_EXCEPTION_INVALID_CHILD_POSITION
#define NODE_EXCEPTION_HEADER "NODE"
#define SHOW_NODE_EXCEPTION_HEADER true

#define NODE_EXCEPTION_STRING(_T_)\
	(_T_ > MAX_NODE_EXCEPTION ? UNKNOWN_EXCEPTION : NODE_EXCEPTION_STR[_T_])
#define THROW_NODE_EXCEPTION(_T_)\
	THROW_EXCEPTION(NODE_EXCEPTION_HEADER, NODE_EXCEPTION_STRING(_T_), SHOW_NODE_EXCEPTION_HEADER)
#define THROW_NODE_EXCEPTION_WITH_MESSAGE(_T_, _M_)\
	THROW_EXCEPTION_WITH_MESSAGE(NODE_EXCEPTION_HEADER, NODE_EXCEPTION_STRING(_T_), _M_, SHOW_NODE_EXCEPTION_HEADER)

class _node;
typedef _node node, *node_ptr;

#endif