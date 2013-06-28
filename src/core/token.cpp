/*
 * token.cpp
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
#include "lexer_base_type.h"
#include "token.h"
#include "token_type.h"

_token::_token(void) :
	_id(uuid::generate_long_id(false))
{
	clear();
}

_token::_token(
	size_t type
	) :
		_id(uuid::generate_long_id(false)),
		_line(INVALID_TYPE),
		_mode(INVALID_TYPE),
		_subtype(INVALID_TYPE),
		_type(type)
{
	return;
}

_token::_token(
	size_t type,
	size_t subtype
	) :
		_id(uuid::generate_long_id(false)),
		_line(INVALID_TYPE),
		_mode(INVALID_TYPE),
		_subtype(subtype),
		_type(type)
{
	return;
}

_token::_token(
	const _token &other
	) :
		_id(other._id),
		_line(other._line),
		_mode(other._mode),
		_origin(other._origin),
		_subtype(other._subtype),
		_text(other._text),
		_type(other._type)
{

}

_token::~_token(void)
{
	return;
}

_token &
_token::operator=(
	const _token &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	if(this != &other) {
		_id = other._id;
		_line = other._line;
		_mode = other._mode;
		_origin = other._origin;
		_subtype = other._subtype;
		_text = other._text;
		_type = other._type;
	}

	return *this;
}

void 
_token::clear(void)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	_line = INVALID_TYPE;
	_mode = INVALID_TYPE;
	_origin.clear();
	_subtype = INVALID_TYPE;
	_text.clear();
	_type = INVALID_TYPE;
}

uuidl_t 
_token::get_id(void)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	return _id;
}

size_t 
_token::get_line(void)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);
	
	return _line;
}

size_t 
_token::get_mode(void)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	return _mode;
}

std::string &
_token::get_origin(void)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	return _origin;
}

size_t 
_token::get_subtype(void)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	return _subtype;
}

std::string &
_token::get_text(void)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	return _text;
}

size_t 
_token::get_type(void)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	return _type;
}

bool 
_token::has_mode(void)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	return _mode != INVALID_TYPE;
}

bool 
_token::has_origin(void)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	return !_origin.empty();
}

bool 
_token::has_text(void)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	return !_text.empty();
}

void 
_token::set_line(
	size_t line
	)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	_line = line;
}

void 
_token::set_mode(
	size_t mode
	)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	_mode = mode;
}

void 
_token::set_origin(
	const std::string &origin
	)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	_origin = origin;
}

void 
_token::set_subtype(
	size_t subtype
	)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	_subtype = subtype;
}

void 
_token::set_text(
	const std::string &text
	)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	_text = text;
}

void 
_token::set_type(
	size_t type
	)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	_type = type;
}

std::string 
_token::to_string(
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, _token_lock);

	std::stringstream ss;

	if(verbose) {
		ss << "{" << LONG_ID_AS_STRING(_id) << "} ";
	}
	ss << "[" << TOKEN_STRING(_type);

	if(_subtype != INVALID_TYPE
			&& TOKEN_TYPE_LENGTH(_type) > _subtype) {
		ss << ", " << TOKEN_TYPE_STRING(_type)[_subtype];
	}

	if(_mode != INVALID_TYPE
			&& _type == TOKEN_OPERAND) {
		ss << ", " << ADDRESSING_MODE_STRING(_mode);
	}
	ss << "]";

	if(!_text.empty()
			&& _text.front() != CHARACTER_END_STREAM) {
		ss << " \'" << _text << "\'";;
	}

	if(_line != INVALID_TYPE) {
		ss << " {" << "ln. " << (_line + 1);
	}

	if(verbose
			&& _line != INVALID_TYPE
			&& !_origin.empty()) {
		ss << ", " << "src. \'" << _origin << "\'";
	}

	if(_line != INVALID_TYPE) {
		ss << "}";
	}

	return ss.str();
}