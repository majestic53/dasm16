/*
 * node.h
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

#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include "type.h"
#include "uuid.h"

typedef class _node {

	public:

		_node(void);

		_node(
			const uuidl_t &id
			);

		_node(
			const uuidl_t &id,
			size_t parent
			);

		_node(
			const _node &other
			);

		virtual ~_node(void);

		_node &operator=(
			const _node &other
			);

		void add_child_position(
			size_t child_position
			);

		virtual void clear(void);

		size_t get_child_position(
			size_t position
			);

		uuidl_t get_id(void);

		size_t get_parent_position(void);

		bool has_children(void);

		bool has_parent(void);

		void insert_child_position(
			size_t child_position,
			size_t position
			);

		void remove_child_position(
			size_t position
			);

		void set_child_position(
			size_t child_position,
			size_t position
			);

		void set_id(
			const uuidl_t &id
			);

		void set_parent_position(
			size_t position
			);

		virtual size_t size(void);

		virtual std::string to_string(
			bool verbose
			);

	protected:

		uuidl_t _id;

		size_t _parent_position;

		std::vector<size_t> _child_position;

	private:

		std::recursive_mutex _node_lock;

} node, *node_ptr;

#endif