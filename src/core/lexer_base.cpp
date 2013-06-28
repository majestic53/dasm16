/*
 * lexer_base.cpp
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

#include <fstream>
#include "lexer_base.h"
#include "lexer_base_type.h"

_lexer_base::_lexer_base(void)
{
	clear();
}

_lexer_base::_lexer_base(
	const std::string &input,
	bool is_file
	)
{
	initialize(input, is_file);
}

_lexer_base::_lexer_base(
	const _lexer_base &other
	)
{
	initialize(other);
}

_lexer_base::~_lexer_base(void)
{
	return;
}

_lexer_base &
_lexer_base::operator=(
	const _lexer_base &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	if(this != &other) {
		initialize(other);
	}

	return *this;
}

void 
_lexer_base::_determine_character_type(void)
{
	char ch = get_character();
	
	if(IS_CHARACTER_ALPHA(ch)) {
		_type = CHARACTER_ALPHA;
	} else if(IS_CHARACTER_DIGIT(ch)) {
		_type = CHARACTER_DIGIT;
	} else if(IS_CHARACTER_WHITESPACE(ch)) {
		_type = CHARACTER_WHITESPACE;
	} else {
		_type = CHARACTER_SYMBOL;
	}
}

void 
_lexer_base::clear(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	_column = 0;
	_column_length.clear();
	_input.clear();
	_input += CHARACTER_END_STREAM;
	_origin.clear();
	_position = 0;
	_row = 0;
	_determine_character_type();
}

char 
_lexer_base::get_character(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	if(_position >= _input.size()) {
		THROW_LEXER_BASE_EXCEPTION_WITH_MESSAGE(
			LEXER_BASE_EXCEPTION_INVALID_CHARACTER_POSITION,
			POSITION_STRING(_position, _row, _column)
			);
	}

	return _input.at(_position);
}

size_t 
_lexer_base::get_character_column(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	return _column;
}

size_t 
_lexer_base::get_character_position(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	return _position;
}

size_t 
_lexer_base::get_character_row(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	return _row;
}

size_t 
_lexer_base::get_character_type(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	return _type;
}

std::string 
_lexer_base::get_origin(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	return _origin;
}

bool 
_lexer_base::has_next_character(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	return get_character() != CHARACTER_END_STREAM;
}

bool 
_lexer_base::has_previous_character(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	return _position > 0;
}

void 
_lexer_base::initialize(
	const std::string &input,
	bool is_file
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	std::stringstream ss;

	clear();

	if(is_file) {
		std::ifstream file(input.c_str(), std::ios::in);

		if(!file) {
			THROW_LEXER_BASE_EXCEPTION_WITH_MESSAGE(
				LEXER_BASE_EXCEPTION_FILE_NOT_FOUND,
				"\'" << input << "\'"
				);
		}
		ss << file.rdbuf();
		_input = ss.str();
		_origin = input;
		file.close();
	} else {
		_input = input;
	}
	_input += CHARACTER_END_STREAM;
	_determine_character_type();
}

void 
_lexer_base::initialize(
	const _lexer_base &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	_column = other._column;
	_column_length = other._column_length;
	_input = other._input;
	_origin = other._origin;
	_position = other._position;
	_row = other._row;
	_type = other._type;
}

char 
_lexer_base::move_next_character(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	char ch;
	std::map<size_t, size_t>::iterator column_length_iter;

	if(!has_next_character()) {
		THROW_LEXER_BASE_EXCEPTION_WITH_MESSAGE(
			LEXER_BASE_EXCEPTION_NO_NEXT_CHARACTER,
			POSITION_STRING(_position, _row, _column)
			);
	}
	ch = get_character();

	if(ch == CHARACTER_NEWLINE) {
		column_length_iter = _column_length.find(_row);

		if(column_length_iter == _column_length.end()) {
			_column_length.insert(std::pair<size_t, size_t>(_row++, _column));
		}
		_column = 0;
	} else {
		++_column;
	}
	++_position;
	_determine_character_type();

	return get_character();
}

char 
_lexer_base::move_previous_character(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	char ch;
	std::map<size_t, size_t>::iterator column_length_iter;

	if(!has_previous_character()) {
		THROW_LEXER_BASE_EXCEPTION_WITH_MESSAGE(
			LEXER_BASE_EXCEPTION_NO_PREVIOUS_CHARACTER,
			POSITION_STRING(_position, _row, _column)
			);
	}
	--_position;
	ch = get_character();

	if(ch == CHARACTER_NEWLINE) {
		column_length_iter = _column_length.find(--_row);

		if(column_length_iter == _column_length.end()) {
			THROW_LEXER_BASE_EXCEPTION_WITH_MESSAGE(
				LEXER_BASE_EXCEPTION_INVALID_COLUMN_LENGTH,
				"row. " << _row
				);
		}
		_column = column_length_iter->second;
	} else {
		--_column;
	}
	_determine_character_type();

	return get_character();
}

void 
_lexer_base::reset(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	_column = 0;
	_position = 0;
	_row = 0;
	_determine_character_type();
}

size_t 
_lexer_base::size(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);
	
	return _input.size() - 1;
}

std::string 
_lexer_base::to_string(
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_base_lock);

	std::stringstream ss;
	char ch = get_character();

	if(verbose) {
		ss << "[" << CHARACTER_TYPE_STRING(_type) << "] ";
	}

	if(_type != CHARACTER_WHITESPACE
			&& ch != CHARACTER_END_STREAM) {
		ss << "\'" << ch << "\' (" << VALUE_AS_HEX(byte_t, ch) << ") ";
	}
	ss << POSITION_STRING(_position, _row, _column);


	return ss.str();
}