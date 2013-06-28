/*
 * parser.cpp
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

#include <queue>
#include "lang_def.h"
#include "parser.h"
#include "parser_type.h"

_parser::_parser(void)
{
	clear();
}

_parser::_parser(
	const std::string &input,
	bool is_file
	)
{
	initialize(input, is_file);
}

_parser::_parser(
	const _parser &other
	)
{
	initialize(other);
}

_parser::~_parser(void)
{
	return;
}

_parser &
_parser::operator=(
	const _parser &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	if(this != &other) {
		initialize(other);
	}

	return *this;
}

void 
_parser::_advance_token(
	bool expect_eos
	)
{
	if(!expect_eos
			&& !has_next_token()) {
		THROW_PARSER_EXCEPTION_WITH_MESSAGE(
			PARSER_EXCEPTION_UNEXPECTED_END_OF_TOKEN_STREAM,
			lexer::to_string(false)
			);
	} else if(expect_eos
			&& has_next_token()) {
		THROW_PARSER_EXCEPTION_WITH_MESSAGE(
			PARSER_EXCEPTION_EXPECTED_END_OF_TOKEN_STREAM,
			lexer::to_string(false)
			);
	}
	move_next_token();
}

size_t 
_parser::_append_token(
	std::vector<node> &statement,
	size_t parent_position
	)
{
	return _append_token(get_token().get_id(), statement, parent_position);
}

size_t 
_parser::_append_token(
	const uuidl_t &id,
	std::vector<node> &statement,
	size_t parent_position
	)
{
	statement.push_back(id);

	if(parent_position != INVALID_TYPE) {

		if(parent_position >= statement.size()) {
			THROW_PARSER_EXCEPTION_WITH_MESSAGE(
				PARSER_EXCEPTION_INVALID_PARENT_POSITION,
				"pos. " << parent_position
				);
		}
		statement.at(parent_position).add_child_position(statement.size() - 1);
		statement.back().set_parent_position(parent_position);
	}

	return statement.size() - 1;
}

void 
_parser::_enumerate_command(
	std::vector<node> &statement
	)
{
	token tok = get_token();
	size_t parent_position = INVALID_TYPE;

	if(tok.get_type() != TOKEN_BASIC_OPCODE
			&& tok.get_type() != TOKEN_SPECIAL_OPCODE) {
		THROW_PARSER_EXCEPTION_WITH_MESSAGE(
			PARSER_EXCEPTION_EXPECTED_COMMAND,
			lexer::to_string(false)
			);
	}
	parent_position = _append_token(statement);
	_advance_token();

	if(tok.get_type() == TOKEN_BASIC_OPCODE) {
		_enumerate_operand_b(statement, parent_position);
		tok = get_token();

		if(tok.get_type() != TOKEN_SYMBOL
				|| (tok.get_type() == TOKEN_SYMBOL
						&& tok.get_subtype() != SYMBOL_LIST_SEPERATOR)) {
			THROW_PARSER_EXCEPTION_WITH_MESSAGE(
				PARSER_EXCEPTION_EXPECTED_SEPERATOR,
				lexer::to_string(false)
				);
		}
		_advance_token();
	}
	_enumerate_operand_a(statement, parent_position);
}

void 
_parser::_enumerate_directive(
	std::vector<node> &statement
	)
{
	token tok = get_token();
	size_t parent_position = INVALID_TYPE;

	if(tok.get_type() != TOKEN_DIRECTIVE) {
		THROW_PARSER_EXCEPTION_WITH_MESSAGE(
			PARSER_EXCEPTION_EXPECTED_DIRECTIVE,
			lexer::to_string(false)
			);
	}
	parent_position = _append_token(statement);
	_advance_token();
	_enumerate_value_list(statement, parent_position);
}

void 
_parser::_enumerate_label(
	std::vector<node> &statement
	)
{
	token tok = get_token();

	if(tok.get_type() != TOKEN_SYMBOL
			|| (tok.get_type() == TOKEN_SYMBOL
					&& tok.get_subtype() != SYMBOL_LABEL_DELIMITOR)) {
		THROW_PARSER_EXCEPTION_WITH_MESSAGE(
			PARSER_EXCEPTION_EXPECTED_LABEL,
			lexer::to_string(false)
			);
	}
	_advance_token();
	tok = get_token();

	if(tok.get_type() != TOKEN_LABEL) {
		THROW_PARSER_EXCEPTION_WITH_MESSAGE(
			PARSER_EXCEPTION_EXPECTED_LABEL,
			lexer::to_string(false)
			);
	}
	_append_token(statement);
	_advance_token();
}

void 
_parser::_enumerate_operand(
	std::vector<node> &statement,
	size_t parent_position
	)
{
	token tok = get_token();
	bool found_parenthesis = false;

	switch(tok.get_type()) {
		case TOKEN_OPERATOR:

			switch(tok.get_subtype()) {
				case OPERATOR_PEEK:
					_set_operand_mode(statement, parent_position, ADDRESSING_MODE_PEEK);
					_advance_token();
					break;
				case OPERATOR_PICK:
					_set_operand_mode(statement, parent_position, ADDRESSING_MODE_PICK);
					_advance_token();
					tok = get_token();

					if(tok.get_type() == TOKEN_SYMBOL 
							&& tok.get_subtype() == SYMBOL_OPEN_PARENTHESIS) {
						found_parenthesis = true;
						_advance_token();
					}
					_enumerate_static_value(statement, parent_position);
					
					if(found_parenthesis) {
						tok = get_token();
						
						if(tok.get_type() != TOKEN_SYMBOL
								|| (tok.get_type() == TOKEN_SYMBOL
										&& tok.get_subtype() != SYMBOL_CLOSE_PARENTHESIS)) {
							THROW_PARSER_EXCEPTION_WITH_MESSAGE(
								PARSER_EXCEPTION_EXPECTED_CLOSE_PARENTHESIS,
								lexer::to_string(false)
								);
						}
						_advance_token();
					}
					break;
				default:
					THROW_PARSER_EXCEPTION_WITH_MESSAGE(
						PARSER_EXCEPTION_ILLEGAL_OPERATOR,
						lexer::to_string(false)
						);
			}
			break;
		case TOKEN_REGISTER:
			_set_operand_mode(statement, parent_position, ADDRESSING_MODE_REGISTER);
			_append_token(statement, parent_position);
			_advance_token();
			break;
		case TOKEN_SYMBOL:

			if(tok.get_subtype() != SYMBOL_OPEN_BRACE) {
				THROW_PARSER_EXCEPTION_WITH_MESSAGE(
					PARSER_EXCEPTION_EXPECTED_OPEN_BRACE,
					lexer::to_string(false)
					);
			}
			_advance_token();
			tok = get_token();

			if(tok.get_type() == TOKEN_REGISTER) {
				_set_operand_mode(statement, parent_position, ADDRESSING_MODE_INDIRECT_REGISTER);
				_append_token(statement, parent_position);
				_advance_token();
				tok = get_token();

				if(tok.get_type() == TOKEN_SYMBOL
						&& tok.get_subtype() == SYMBOL_ADD_NEXT) {
					_set_operand_mode(statement, parent_position, ADDRESSING_MODE_INDIRECT_REGISTER_NEXT_WORD);
					_advance_token();
					_enumerate_static_value(statement, parent_position);
				}
			} else {
				_set_operand_mode(statement, parent_position, ADDRESSING_MODE_INDIRECT_NEXT_WORD);
				_enumerate_static_value(statement, parent_position);
				tok = get_token();

				if(tok.get_type() == TOKEN_SYMBOL
						&& tok.get_subtype() == SYMBOL_ADD_NEXT) {
					_set_operand_mode(statement, parent_position, ADDRESSING_MODE_INDIRECT_REGISTER_NEXT_WORD);
					_advance_token();
					tok = get_token();
					
					if(tok.get_type() != TOKEN_REGISTER) {
						THROW_PARSER_EXCEPTION_WITH_MESSAGE(
							PARSER_EXCEPTION_EXPECTED_REGISTER,
							lexer::to_string(false)
							);
					}
					_append_token(statement, parent_position);
					_advance_token();
				}
			}
			tok = get_token();

			if(tok.get_type() != TOKEN_SYMBOL
					|| (tok.get_type() == TOKEN_SYMBOL
							&& tok.get_subtype() != SYMBOL_CLOSE_BRACE)) {
				THROW_PARSER_EXCEPTION_WITH_MESSAGE(
					PARSER_EXCEPTION_EXPECTED_CLOSE_BRACE,
					lexer::to_string(false)
					);
			}
			_advance_token();
			break;
		case TOKEN_SYSTEM_REGISTER:

			switch(tok.get_subtype()) {
				case SYSTEM_REGISTER_EX:
					_set_operand_mode(statement, parent_position, ADDRESSING_MODE_EX_LITERAL);
					break;
				case SYSTEM_REGISTER_PC:
					_set_operand_mode(statement, parent_position, ADDRESSING_MODE_PC_LITERAL);
					break;
				case SYSTEM_REGISTER_SP:
					_set_operand_mode(statement, parent_position, ADDRESSING_MODE_SP_LITERAL);
					break;
				default:
					THROW_PARSER_EXCEPTION_WITH_MESSAGE(
						PARSER_EXCEPTION_ILLEGAL_SYSTEM_REGISTER,
						lexer::to_string(false)
						);
			}
			_advance_token();
			break;
		default:
			_set_operand_mode(statement, parent_position, ADDRESSING_MODE_NEXT_WORD_LITERAL);
			_enumerate_static_value(statement, parent_position);
			break;
	}
}

void 
_parser::_enumerate_operand_a(
	std::vector<node> &statement,
	size_t parent_position
	)
{
	token tok, oper_tok(TOKEN_OPERAND);

	parent_position = _append_token(insert_token(oper_tok), statement, parent_position);
	_advance_token();
	tok = get_token();

	if(tok.get_type() == TOKEN_SYMBOL
			&& tok.get_subtype() == SYMBOL_LITERAL_VALUE_DELIMITOR) {
		_set_operand_mode(statement, parent_position, ADDRESSING_MODE_LITERAL);
		_advance_token();
		_enumerate_static_value(statement, parent_position);
	} else if(tok.get_type() == TOKEN_OPERATOR
			&& tok.get_subtype() == OPERATOR_POP) {
		_set_operand_mode(statement, parent_position, ADDRESSING_MODE_PUSH_POP);
		_append_token(statement, parent_position);
		_advance_token();
	} else {
		_enumerate_operand(statement, parent_position);
	}
}

void 
_parser::_enumerate_operand_b(
	std::vector<node> &statement,
	size_t parent_position
	)
{
	token tok, oper_tok(TOKEN_OPERAND);

	parent_position = _append_token(insert_token(oper_tok), statement, parent_position);
	_advance_token();
	tok = get_token();

	if(tok.get_type() == TOKEN_OPERATOR
			&& tok.get_subtype() == OPERATOR_PUSH) {
		_set_operand_mode(statement, parent_position, ADDRESSING_MODE_PUSH_POP);
		_append_token(statement, parent_position);
		_advance_token();
	} else {
		_enumerate_operand(statement, parent_position);
	}
}

void 
_parser::_enumerate_statement(
	std::vector<node> &statement
	)
{
	token tok = get_token();

	statement.clear();

	switch(tok.get_type()) {
		case TOKEN_BASIC_OPCODE:
		case TOKEN_SPECIAL_OPCODE:
			_enumerate_command(statement);
			break;
		case TOKEN_DIRECTIVE:
			_enumerate_directive(statement);
			break;
		case TOKEN_SYMBOL:

			switch(tok.get_subtype()) {
				case SYMBOL_LABEL_DELIMITOR:
					_enumerate_label(statement);
					break;
				default:
					THROW_PARSER_EXCEPTION_WITH_MESSAGE(
						PARSER_EXCEPTION_EXPECTED_STATEMENT,
						lexer::to_string(false)
						);
			}
			break;
		default:
			THROW_PARSER_EXCEPTION_WITH_MESSAGE(
				PARSER_EXCEPTION_EXPECTED_STATEMENT,
				lexer::to_string(false)
				);
	}
}

void 
_parser::_enumerate_static_value(
	std::vector<node> &statement,
	size_t parent_position
	)
{
	token tok = get_token();

	switch(tok.get_type()) {
		case TOKEN_LABEL:
		case TOKEN_VALUE:

			if(tok.get_subtype() == VALUE_STRING_VAR) {
				THROW_PARSER_EXCEPTION_WITH_MESSAGE(
					PARSER_EXCEPTION_EXPECTED_STATIC_VALUE,
					lexer::to_string(false)
					);				
			}
			_append_token(statement, parent_position);
			_advance_token();
			break;
		default:
			THROW_PARSER_EXCEPTION_WITH_MESSAGE(
				PARSER_EXCEPTION_EXPECTED_STATIC_VALUE,
				lexer::to_string(false)
				);
	}
}

void 
_parser::_enumerate_value(
	std::vector<node> &statement,
	size_t parent_position
	)
{
	token tok = get_token();

	switch(tok.get_type()) {
		case TOKEN_LABEL:
		case TOKEN_VALUE:
			_append_token(statement, parent_position);
			_advance_token();
			break;
		default:
			THROW_PARSER_EXCEPTION_WITH_MESSAGE(
				PARSER_EXCEPTION_EXPECTED_VALUE,
				lexer::to_string(false)
				);
	}
}

void 
_parser::_enumerate_value_list(
	std::vector<node> &statement,
	size_t parent_position
	)
{
	token tok, list_tok(TOKEN_VALUE_LIST);

	parent_position = _append_token(insert_token(list_tok), statement, parent_position);
	_advance_token();
	_enumerate_value(statement, parent_position);
	tok = get_token();

	while(tok.get_type() == TOKEN_SYMBOL
			&& tok.get_subtype() == SYMBOL_LIST_SEPERATOR) {
		_advance_token();
		_enumerate_value(statement, parent_position);
		tok = get_token();
	}
}

void 
_parser::_set_operand_mode(
	std::vector<node> &statement,
	size_t operand_position,
	size_t mode
	)
{
	if(operand_position >= statement.size()) {
		THROW_PARSER_EXCEPTION_WITH_MESSAGE(
			PARSER_EXCEPTION_INVALID_OPERAND_POSITION,
			"pos. " << operand_position
			);
	}
	get_token(statement.at(operand_position).get_id()).set_mode(mode);
}

void 
_parser::clear(void)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	std::vector<node> begin_stmt, end_stmt;
	node begin_node(lexer::get_begin_token_id()),
		end_node(lexer::get_end_token_id());

	lexer::reset();
	begin_stmt.push_back(begin_node);
	end_stmt.push_back(end_node);
	_position = 0;
	_statement.clear();
	_statement.push_back(begin_stmt);
	_statement.push_back(end_stmt);
}

void 
_parser::discover(void)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	clear();

	while(has_next_statement()) {
		move_next_statement();
	}
	reset();
}

std::vector<node> &
_parser::get_statement(void)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	if(_position >= _statement.size()) {
		THROW_PARSER_EXCEPTION_WITH_MESSAGE(
			PARSER_EXCEPTION_INVALID_STATEMENT_POSITION,
			"pos. " << _position
			);
	}

	return _statement.at(_position);
}

std::vector<node> &
_parser::get_statement(
	size_t position
	)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	if(position >= _statement.size()) {
		THROW_PARSER_EXCEPTION_WITH_MESSAGE(
			PARSER_EXCEPTION_INVALID_STATEMENT_POSITION,
			"pos. " << position
			);
	}

	return _statement.at(position);
}

size_t 
_parser::get_statement_position(void)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	return _position;
}

bool 
_parser::has_next_statement(void)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	return lexer::get_token(get_statement().front().get_id()).get_type() != TOKEN_END;
}

bool 
_parser::has_previous_statement(void)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	return _position > 0; 
}

void 
_parser::initialize(
	const std::string &input,
	bool is_file
	)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	lexer::initialize(input, is_file);
	clear();
}

void 
_parser::initialize(
	const _parser &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	lexer::initialize(other);
	_position = other._position;
	_statement = other._statement;
}

std::vector<node> &
_parser::move_next_statement(void)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	std::vector<node> statement;

	if(!has_next_statement()) {
		THROW_PARSER_EXCEPTION_WITH_MESSAGE(
			PARSER_EXCEPTION_NO_NEXT_STATEMENT,
			"pos. " << _position
			);
	}

	if(has_next_token()
			&& _position == (_statement.size() - 2)) {

		if(lexer::get_token().get_type() == TOKEN_BEGIN) {
			_advance_token();
		}

		if(has_next_token()) {
			_enumerate_statement(statement);
			_statement.insert(_statement.begin() + (++_position), statement);
		} else {
			++_position;
		}
	} else if(_position < (_statement.size() - 1)) {
		++_position;
	}

	return get_statement();
}

std::vector<node> &
_parser::move_previous_statement(void)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	if(!has_previous_statement()) {
		THROW_PARSER_EXCEPTION_WITH_MESSAGE(
			PARSER_EXCEPTION_NO_PREVIOUS_STATEMENT,
			"pos. " << _position
			);
	}
	--_position;
	
	return get_statement();
}

void 
_parser::reset(void)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	_position = 0;
}

size_t 
_parser::size(void)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	return _statement.size() - 2;
}

std::string 
_parser::statement_to_string(
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	return statement_to_string(_position, verbose);
}

std::string 
_parser::statement_to_string(
	size_t position,
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	size_t i;
	std::stringstream ss;
	std::queue<node> que;
	std::vector<node> statement = get_statement(position);

	if(!statement.empty()) {
		que.push(statement.front());

		while(!que.empty()) {
			ss << lexer::get_token(que.front().get_id()).to_string(verbose) 
					<< que.front().to_string(false) << std::endl;

			for(i = 0; i < que.front().size(); ++i) {
				que.push(statement.at(que.front().get_child_position(i)));
			}
			que.pop();
		}
	}

	return ss.str();
}

std::string 
_parser::to_string(
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, _parser_lock);

	std::stringstream ss;

	if(verbose) {
		ss << "STATEMENT[" << _position << "]" << std::endl;
	}
	ss << statement_to_string(_position, verbose);

	return ss.str();
}