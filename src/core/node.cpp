/*
 * node.cpp
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

#include "node.h"
#include "node_type.h"

_node::_node(void)
{
	clear();
}

_node::_node(
	const uuidl_t &id
	) :
		_id(id),
		_parent_position(INVALID_TYPE)
{
	return;
}

_node::_node(
	const uuidl_t &id,
	size_t parent
	) :
		_id(id),
		_parent_position(parent)
{
	return;
}

_node::_node(
	const _node &other
	) :
		_child_position(other._child_position),
		_id(other._id),
		_parent_position(other._parent_position)
{
	return;
}

_node::~_node(void)
{
	return;
}

_node &
_node::operator=(
	const _node &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	if(this != &other) {
		_child_position = other._child_position;
		_id = other._id;
		_parent_position = other._parent_position;
	}

	return *this;
}

void 
	_node::add_child_position(
	size_t child_position
	)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	_child_position.push_back(child_position);
}

void 
_node::clear(void)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	_child_position.clear();
	_id = uuid::NULL_LONG_ID;
	_parent_position = INVALID_TYPE;
}

size_t 
_node::get_child_position(
	size_t position
	)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	if(position >= _child_position.size()) {
		THROW_NODE_EXCEPTION_WITH_MESSAGE(
			NODE_EXCEPTION_INVALID_CHILD_POSITION,
			"pos. " << position
			);
	}

	return _child_position.at(position);
}

uuidl_t 
_node::get_id(void)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	return _id;
}

size_t 
_node::get_parent_position(void)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	return _parent_position;
}

bool 
_node::has_children(void)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	return !_child_position.empty();
}

bool 
_node::has_parent(void)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	return _parent_position != INVALID_TYPE; 
}

void 
_node::insert_child_position(
	size_t child_position,
	size_t position
	)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	if(position > _child_position.size()) {
		THROW_NODE_EXCEPTION_WITH_MESSAGE(
			NODE_EXCEPTION_INVALID_CHILD_POSITION,
			"pos. " << position
			);
	}

	if(position == _child_position.size()) {
		_child_position.push_back(child_position);
	} else {
		_child_position.insert(_child_position.begin() + position, child_position);
	}
}

void 
_node::remove_child_position(
	size_t position
	)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	if(position >= _child_position.size()) {
		THROW_NODE_EXCEPTION_WITH_MESSAGE(
			NODE_EXCEPTION_INVALID_CHILD_POSITION,
			"pos. " << position
			);
	}
	_child_position.erase(_child_position.begin() + position);
}

void 
_node::set_child_position(
	size_t child_position,
	size_t position
	)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	if(position >= _child_position.size()) {
		THROW_NODE_EXCEPTION_WITH_MESSAGE(
			NODE_EXCEPTION_INVALID_CHILD_POSITION,
			"pos. " << position
			);
	}
	_child_position.at(position) = child_position;
}

void 
_node::set_id(
	const uuidl_t &id
	)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	_id = id;
}

void 
_node::set_parent_position(
	size_t position
	)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);
	
	_parent_position = position;
}

size_t 
_node::size(void)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	return _child_position.size();
}

std::string 
_node::to_string(
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, _node_lock);

	std::stringstream ss;

	if(verbose) {
		ss << "{" << LONG_ID_AS_STRING(_id) << "} ";
	}
	ss << " (" << _child_position.size() << ")";

	if(!has_parent()) {
		ss << " [ROOT]";
	}

	if(!has_children()) {
		ss << " [LEAF]";
	}

	return ss.str();
}