/*
 * lexer.cpp
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
#include "lexer.h"
#include "lexer_base_type.h"
#include "lexer_type.h"

_lexer::_lexer(void)
{
	clear();
}

_lexer::_lexer(
	const std::string &input,
	bool is_file
	)
{
	initialize(input, is_file);
}

_lexer::_lexer(
	const _lexer &other
	)
{
	initialize(other);
}

_lexer::~_lexer(void)
{
	return;
}

_lexer &
_lexer::operator=(
	const _lexer &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	if(this != &other) {
		initialize(other);
	}

	return *this;
}

void 
_lexer::_advance_character(
	bool expect_eos
	)
{
	if(!expect_eos
			&& !has_next_character()) {
		THROW_LEXER_EXCEPTION_WITH_MESSAGE(
			LEXER_EXCEPTION_UNEXPECTED_END_OF_CHARACTER_STREAM,
			lexer_base::to_string(false)
			);
	} else if(expect_eos
			&& has_next_character()) {
		THROW_LEXER_EXCEPTION_WITH_MESSAGE(
			LEXER_EXCEPTION_EXPECTED_END_OF_CHARACTER_STREAM,
			lexer_base::to_string(false)
			);
	}
	move_next_character();
}

void 
_lexer::_enumerate_alpha_token(
	token &tok
	)
{
	char ch;
	size_t type;

	tok.clear();

	if(get_character_type() != CHARACTER_ALPHA
			&& get_character() != CHARACTER_UNDERSCORE) {
		THROW_LEXER_EXCEPTION_WITH_MESSAGE(
			LEXER_EXCEPTION_EXPECTED_ALPHA_CHARACTER,
			lexer_base::to_string(false)
			);
	}
	tok.get_text() += get_character();
	_advance_character();

	while(has_next_character()) {
		ch = get_character();
		type = get_character_type();

		if(type != CHARACTER_ALPHA
				&& type != CHARACTER_DIGIT
				&& ch != CHARACTER_UNDERSCORE) {
			break;
		}
		tok.get_text() += ch;
		_advance_character();
	}
	TO_UPPERCASE(tok.get_text());

	if(IS_DIRECTIVE(tok.get_text())) {
		tok.set_type(TOKEN_DIRECTIVE);
	} else if(IS_OPCODE_BASIC(tok.get_text())) {
		tok.set_type(TOKEN_BASIC_OPCODE);
	} else if(IS_OPCODE_SPECIAL(tok.get_text())) {
		tok.set_type(TOKEN_SPECIAL_OPCODE);
	} else if(IS_OPERATOR(tok.get_text())) {
		tok.set_type(TOKEN_OPERATOR);
	} else if(IS_REGISTER(tok.get_text())) {
		tok.set_type(TOKEN_REGISTER);
	} else if(IS_SYSTEM_REGISTER(tok.get_text())) {
		tok.set_type(TOKEN_SYSTEM_REGISTER);
	} else {
		tok.set_type(TOKEN_LABEL);
	}

	if(tok.get_type() != TOKEN_LABEL) {
		tok.set_subtype(__determine_token_subtype(tok.get_text(), tok.get_type()));
		tok.get_text().clear();
	}
}

void 
_lexer::_enumerate_digit_token(
	token &tok
	)
{
	char ch;
	tok.clear();
	tok.set_type(TOKEN_VALUE);

	if(get_character_type() != CHARACTER_DIGIT) {
		THROW_LEXER_EXCEPTION_WITH_MESSAGE(
			LEXER_EXCEPTION_EXPECTED_DIGIT_CHARACTER,
			lexer_base::to_string(false)
			);
	}
	ch = get_character();
	tok.get_text() += get_character();
	_advance_character();

	if(ch == CHARACTER_ZERO
			&& (get_character() == CHARACTER_HEXIDECIMAL_DELIMITOR_LOWER
					|| get_character() == CHARACTER_HEXIDECIMAL_DELIMITOR_UPPER)) {
		tok.set_subtype(VALUE_HEXIDECIMAL);
		tok.get_text().clear();
		_advance_character();

		if(!IS_VALUE_TYPE(get_character())) {
			THROW_LEXER_EXCEPTION_WITH_MESSAGE(
				LEXER_EXCEPTION_EXPECTED_DIGIT_CHARACTER,
				lexer_base::to_string(false)
				);
		}
	} else {
		tok.set_subtype(VALUE_INTEGER);
	}

	while(has_next_character()) {
		ch = get_character();

		if(!IS_VALUE_TYPE(ch)
				|| (tok.get_subtype() == VALUE_INTEGER 
						&& IS_HEXIDECIMAL_CHARACTER(ch))) {
			break;
		}
		tok.get_text() += ch;
		_advance_character();
	}
}

void 
_lexer::_enumerate_symbol_token(
	token &tok
	)
{
	char ch;
	size_t position;

	tok.clear();

	if(get_character_type() != CHARACTER_SYMBOL) {
		THROW_LEXER_EXCEPTION_WITH_MESSAGE(
			LEXER_EXCEPTION_EXPECTED_SYMBOL_CHARACTER,
			lexer_base::to_string(false)
			);
	}
	ch = get_character();
	
	switch(ch) {
		case CHARACTER_STRING_DELIMITOR:
			tok.set_type(TOKEN_VALUE);
			tok.set_subtype(VALUE_STRING_VAR);
			position = get_character_position();

			if(!has_next_character()) {
				THROW_LEXER_EXCEPTION_WITH_MESSAGE(
					LEXER_EXCEPTION_UNTERMINATED_STRING_LITERAL,
					"pos. " << position
					);
			}
			_advance_character();

			while(has_next_character()) {
				ch = get_character();

				if(ch == CHARACTER_STRING_DELIMITOR) {
					break;
				}
				tok.get_text() += ch;
				_advance_character();
			}

			if(!has_next_character()
					&& ch != CHARACTER_STRING_DELIMITOR) {
				THROW_LEXER_EXCEPTION_WITH_MESSAGE(
					LEXER_EXCEPTION_UNTERMINATED_STRING_LITERAL,
					"pos. " << position
					);
			}
			_advance_character();
			break;
		case CHARACTER_UNDERSCORE:
			_enumerate_alpha_token(tok);
			break;
		default:

			if(IS_SYMBOL_CHARACTER(ch)) {
				tok.set_type(TOKEN_SYMBOL);
				tok.get_text() += get_character();
				tok.set_subtype(__determine_token_subtype(tok.get_text(), tok.get_type()));
				tok.get_text().clear();
				_advance_character();
			} else {
				THROW_LEXER_EXCEPTION_WITH_MESSAGE(
					LEXER_EXCEPTION_UNKNOWN_SYMBOL_CHARACTER,
					lexer_base::to_string(false)
					);
			}
			break;
	}
}

void 
_lexer::_skip_whitespace(void)
{
	while(has_next_character()) {

		if(get_character_type() != CHARACTER_WHITESPACE) {
			break;
		}
		move_next_character();
	}

	if(has_next_character()
			&& get_character_type() == CHARACTER_SYMBOL
			&& get_character() == CHARACTER_COMMENT) {

		while(has_next_character()) {

			if(get_character_type() == CHARACTER_WHITESPACE
					&& get_character() == CHARACTER_NEWLINE) {
				break;
			}
			move_next_character();
		}
		_skip_whitespace();
	}
}

void 
_lexer::clear(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	token begin_tok(TOKEN_BEGIN), end_tok(TOKEN_END);

	lexer_base::reset();
	_position = 0;
	_token.clear();
	_token_position.clear();
	_token.insert(std::pair<uuidl_t, token>(begin_tok.get_id(), begin_tok));
	_token.insert(std::pair<uuidl_t, token>(end_tok.get_id(), end_tok));
	_token_position.push_back(begin_tok.get_id());
	_token_position.push_back(end_tok.get_id());
}

void 
_lexer::discover(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	clear();

	while(has_next_token()) {
		move_next_token();
	}
	reset();
}

std::vector<token> 
_lexer::export_tokens(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	token tok;
	std::vector<token> result;
	std::vector<uuidl_t>::iterator token_pos_iter = _token_position.begin();

	for(; token_pos_iter != _token_position.end(); ++token_pos_iter) {
		tok = _token.find(*token_pos_iter)->second;

		if(tok.get_type() == TOKEN_BEGIN
				|| tok.get_type() == TOKEN_END) {
			continue;
		}
		result.push_back(tok);
	}

	return result;
}

uuidl_t 
_lexer::get_begin_token_id(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	return _token_position.front();
}

uuidl_t 
_lexer::get_end_token_id(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	return _token_position.back();
}

token &
_lexer::get_token(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	std::map<uuidl_t, token>::iterator token_iter;

	if(_position >= _token.size()) {
		THROW_LEXER_EXCEPTION_WITH_MESSAGE(
			LEXER_EXCEPTION_INVALID_TOKEN_POSITION,
			"pos. " << _position
			);
	}
	token_iter = _token.find(_token_position.at(_position));

	if(token_iter == _token.end()) {
		THROW_LEXER_EXCEPTION_WITH_MESSAGE(
			LEXER_EXCEPTION_TOKEN_NOT_FOUND,
			"{" << LONG_ID_AS_STRING(_token_position.at(_position)) << "}"
			);
	}

	return token_iter->second;
}

token &
_lexer::get_token(
	uuidl_t id
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	std::map<uuidl_t, token>::iterator token_iter = _token.find(id);

	if(token_iter == _token.end()) {
		THROW_LEXER_EXCEPTION_WITH_MESSAGE(
			LEXER_EXCEPTION_TOKEN_NOT_FOUND,
			"{" << LONG_ID_AS_STRING(id) << "}"
			);
	}

	return token_iter->second;
}

size_t 
_lexer::get_token_position(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	return _position;
}

bool 
_lexer::has_token(
	uuidl_t id
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	return _token.find(id) != _token.end();
}

bool 
_lexer::has_next_token(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	return get_token().get_type() != TOKEN_END;
}

bool 
_lexer::has_previous_token(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	return _position > 0;
}

void 
_lexer::import_tokens(
	std::vector<token> tokens
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	std::vector<token>::iterator token_iter = tokens.begin();

	for(; token_iter != tokens.end(); ++token_iter) {
		_token.insert(std::pair<uuidl_t, token>(token_iter->get_id(), *token_iter));
		_token_position.insert(_token_position.begin() + (_position++), token_iter->get_id());
	}
}

void 
_lexer::initialize(
	const std::string &input,
	bool is_file
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	lexer_base::initialize(input, is_file);
	clear();
}

void 
_lexer::initialize(
	const _lexer &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	lexer_base::initialize(other);
	_position = other._position;
	_token = other._token;
	_token_position = other._token_position;
}

uuidl_t  
_lexer::insert_token(
	token &tok
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	return insert_token(tok, _position);
}

uuidl_t  
_lexer::insert_token(
	token &tok,
	size_t position
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	if(position > _token.size()) {
		THROW_LEXER_EXCEPTION_WITH_MESSAGE(
			LEXER_EXCEPTION_INVALID_TOKEN_POSITION,
			"pos. " << _position
			);
	}
	_token.insert(std::pair<uuidl_t, token>(tok.get_id(), tok));

	if(position == _token.size()) {
		_token_position.push_back(tok.get_id());
	} else {
		_token_position.insert(_token_position.begin() + position, tok.get_id());
	}

	return _token_position.at(position);
}

token &
_lexer::move_next_token(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	token new_token;
	size_t line, type;

	if(!has_next_token()) {
		THROW_LEXER_EXCEPTION_WITH_MESSAGE(
			LEXER_EXCEPTION_NO_NEXT_TOKEN,
			"pos. " << _position
			);
	}
	_skip_whitespace();

	if(has_next_character()
			&& _position == (_token_position.size() - 2)) {
		line = get_character_row();
		type = get_character_type();

		switch(type) {
			case CHARACTER_ALPHA:
				_enumerate_alpha_token(new_token);
				break;
			case CHARACTER_DIGIT:
				_enumerate_digit_token(new_token);
				break;
			case CHARACTER_SYMBOL:
				_enumerate_symbol_token(new_token);
				break;
			default:
				THROW_LEXER_EXCEPTION_WITH_MESSAGE(
					LEXER_EXCEPTION_INVALID_TOKEN_TYPE,
					"type. " << type
					);
		}
		new_token.set_line(line);
		new_token.set_origin(get_origin());
		_token.insert(std::pair<uuidl_t, token>(new_token.get_id(), new_token));
		_token_position.insert(_token_position.begin() + (++_position), new_token.get_id());
	} else if(_position < (_token_position.size() - 1)) {
		++_position;
	}
	
	return get_token();
}

token &
_lexer::move_previous_token(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	if(!has_previous_token()) {
		THROW_LEXER_EXCEPTION_WITH_MESSAGE(
			LEXER_EXCEPTION_NO_PREVIOUS_TOKEN,
			"pos. " << _position
			);
	}
	--_position;

	return get_token();
}

void 
_lexer::reset(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	_position = 0;
}

size_t 
_lexer::size(void)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	return _token_position.size() - 2;
}

std::string 
_lexer::to_string(
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, _lexer_lock);

	std::stringstream ss;

	ss << get_token().to_string(verbose); 
	
	if(verbose) {
		ss << " {token pos. " << _position << "}";
	}

	return ss.str();
}