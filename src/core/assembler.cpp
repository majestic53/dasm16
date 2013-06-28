/*
 * assembler.cpp
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
#include "assembler.h"
#include "assembler_type.h"
#include "lang_def.h"
#include "lexer_type.h"

_assembler::_assembler(void)
{
	clear();
}

_assembler::_assembler(
	const std::string &input,
	bool is_file
	)
{
	initialize(input, is_file);
}

_assembler::_assembler(
	const _assembler &other
	)
{
	initialize(other);
}

_assembler::~_assembler(void)
{
	return;
}

_assembler &
_assembler::operator=(
	const _assembler &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _assembler_lock);

	if(this != &other) {
		initialize(other);
	}

	return *this;
}

std::vector<word_t> & 
_assembler::_evaluate_operand(
	std::vector<node> &statement,
	size_t position
	)
{
	size_t child_position;
	bool found_register = false;
	token oper_token, value_token;
	std::vector<word_t> result, next_word;
	std::map<uuidl_t, std::vector<word_t>>::iterator value_iter;

	if(position >= statement.size()) {
		THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
			ASSEMBLER_EXCEPTION_INVALID_STATEMENT_POSITION,
			"pos. " << position
			);
	}
	oper_token = get_token(statement.at(position).get_id());

	if(oper_token.get_type() != TOKEN_OPERAND) {
		THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
			ASSEMBLER_EXCEPTION_EXPECTED_OPERAND_NODE,
			oper_token.to_string(false)
			);
	}
	value_iter = _value.find(oper_token.get_id());

	if(value_iter == _value.end()) {
		result.push_back(ADDRESSING_MODE_VALUE(oper_token.get_mode()));

		switch(oper_token.get_mode()) {
			case ADDRESSING_MODE_REGISTER:
			case ADDRESSING_MODE_INDIRECT_REGISTER:

				if(!statement.at(position).has_children()) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
						oper_token.to_string(false)
						);
				}
				child_position = statement.at(position).get_child_position(0);

				if(child_position >= statement.size()) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_INVALID_STATEMENT_POSITION,
						"pos. " << child_position
						);
				}
				value_token = get_token(statement.at(child_position).get_id());

				if(value_token.get_type() != TOKEN_REGISTER) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
						value_token.to_string(false)
						);				
				}
				result.front() += (word_t) value_token.get_subtype();
				break;
			case ADDRESSING_MODE_INDIRECT_REGISTER_NEXT_WORD:

				if(!statement.at(position).has_children()) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
						oper_token.to_string(false)
						);
				}
				child_position = statement.at(position).get_child_position(0);

				if(child_position >= statement.size()) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_INVALID_STATEMENT_POSITION,
						"pos. " << child_position
						);
				}
				value_token = get_token(statement.at(child_position).get_id());

				if(value_token.get_type() != TOKEN_REGISTER) {
					next_word = _evaluate_value(get_token(statement.at(child_position).get_id()), false);
					result.insert(result.end(), next_word.begin(), next_word.end());
				} else {
					result.front() += (word_t) value_token.get_subtype();
					found_register = true;
				}
				child_position = statement.at(position).get_child_position(1);

				if(child_position >= statement.size()) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_INVALID_STATEMENT_POSITION,
						"pos. " << child_position
						);
				}
				value_token = get_token(statement.at(child_position).get_id());

				if(found_register) {
					next_word = _evaluate_value(get_token(statement.at(child_position).get_id()), false);
					result.insert(result.end(), next_word.begin(), next_word.end());
				} else {
					result.front() += (word_t) value_token.get_subtype();
				}
				break;
			case ADDRESSING_MODE_PUSH_POP:
			case ADDRESSING_MODE_PEEK:
			case ADDRESSING_MODE_SP_LITERAL:
			case ADDRESSING_MODE_PC_LITERAL:
			case ADDRESSING_MODE_EX_LITERAL:
				break;
			case ADDRESSING_MODE_INDIRECT_NEXT_WORD:
			case ADDRESSING_MODE_LITERAL:
			case ADDRESSING_MODE_NEXT_WORD_LITERAL:
			case ADDRESSING_MODE_PICK:

				if(!statement.at(position).has_children()) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
						oper_token.to_string(false)
						);
				}
				child_position = statement.at(position).get_child_position(0);

				if(child_position >= statement.size()) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_INVALID_STATEMENT_POSITION,
						"pos. " << child_position
						);
				}
				next_word = _evaluate_value(
					get_token(statement.at(child_position).get_id()), 
					oper_token.get_mode() == ADDRESSING_MODE_LITERAL
					);
				
				if(oper_token.get_mode() == ADDRESSING_MODE_LITERAL) {
					
					if(next_word.front() <= MAX_LITERAL) {
						result.back() += (next_word.front() + 1);
					}
				} else {
					result.insert(result.end(), next_word.begin(), next_word.end());
				}
				break;
			default:
				THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
					ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
					oper_token.to_string(false)
					);
		}
		_value.insert(std::pair<uuidl_t, std::vector<word_t>>(oper_token.get_id(), result));
		value_iter = _value.find(oper_token.get_id());
	}

	return value_iter->second;
}

std::vector<word_t> &  
_assembler::_evaluate_value(
	token &value_token,
	bool literal_value
	)
{
	char ch;
	size_t value;
	std::vector<word_t> result;
	std::string::iterator ch_iter;
	size_t i = 0, radix = INTEGER_RADIX;
	std::map<std::string, word_t>::iterator label_iter;
	std::map<uuidl_t, std::vector<word_t>>::iterator value_iter;

	if(value_token.get_type() != TOKEN_VALUE
			&& value_token.get_type() != TOKEN_LABEL) {
		THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
			ASSEMBLER_EXCEPTION_EXPECTED_VALUE_NODE,
			value_token.to_string(false)
			);
	}
	value_iter = _value.find(value_token.get_id());

	if(value_iter == _value.end()) {

		if(value_token.get_type() == TOKEN_LABEL) {
			label_iter = _label_offset.find(value_token.get_text());

			if(label_iter == _label_offset.end()) {
				THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
					ASSEMBLER_EXCEPTION_LABEL_UNDEFINED,
					value_token.to_string(false)
					);
			}
			result.push_back(label_iter->second);
		} else {

			switch(value_token.get_subtype()) {
				case VALUE_HEXIDECIMAL:
				case VALUE_INTEGER:
					value = 0;
					result.push_back(0);

					if(value_token.get_subtype() == VALUE_HEXIDECIMAL) {
						radix = HEXIDECIMAL_RADIX;
					}

					for(; i < value_token.get_text().size(); ++i) {
						ch = value_token.get_text().at(i);

						if(ch >= CHARACTER_HEXIDECIMAL_LOWER_0
								&& ch <= CHARACTER_HEXIDECIMAL_LOWER_1) {
							ch = (ch - CHARACTER_HEXIDECIMAL_LOWER_0) + HEXIDECIMAL_OFFSET;
						} else if(ch >= CHARACTER_HEXIDECIMAL_UPPER_0
								&& ch <= CHARACTER_HEXIDECIMAL_UPPER_1) {
							ch = (ch - CHARACTER_HEXIDECIMAL_UPPER_0) + HEXIDECIMAL_OFFSET;
						} else {
							ch -= CHARACTER_ZERO;
						}
						value += ch * (size_t) std::pow(radix, value_token.get_text().size() - i - 1);

						if(value > MAX_WORD) {
							THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
								ASSEMBLER_EXCEPTION_VALUE_OUT_OR_RANGE,
								value_token.to_string(false)
								);
						}
					}
					result.front() = (word_t) value;
					break;
				case VALUE_STRING_VAR:

					for(ch_iter = value_token.get_text().begin(); ch_iter != value_token.get_text().end(); ++ch_iter) {
						result.push_back((word_t) *ch_iter);
					}
					break;
				default:
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
						value_token.to_string(false)
						);
			}

			if(literal_value
					&& (value_token.get_subtype() == VALUE_HEXIDECIMAL
							|| value_token.get_subtype() == VALUE_INTEGER)
					&& !result.empty()
					&& (result.front() != MIN_LITERAL
							&& result.front() > MAX_LITERAL)) {
				THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
					ASSEMBLER_EXCEPTION_LITERAL_VALUE_OUT_OR_RANGE,
					value_token.to_string(false)
					);
			}
		}
		_value.insert(std::pair<uuidl_t, std::vector<word_t>>(value_token.get_id(), result));
		value_iter = _value.find(value_token.get_id());
	}

	return value_iter->second;
}

void 
_assembler::_generate_pass_0(void)
{
	node value_node;
	word_t offset = 0;
	token tok, value_tok;
	std::vector<word_t> value;
	std::vector<node> statement;
	size_t i, child_position, value_list_position;
	std::map<std::string, word_t>::iterator label_iter;

	parser::reset();
	_label_offset.clear();
	
	while(has_next_statement()) {
		statement = get_statement();
		tok = get_token(statement.front().get_id());

		switch(tok.get_type()) {
			case TOKEN_BEGIN:
				break;
			case TOKEN_BASIC_OPCODE:
			case TOKEN_SPECIAL_OPCODE:
				++offset;

				for(i = 0; i < statement.front().size(); ++i) {
					child_position = statement.front().get_child_position(i);

					if(child_position >= statement.size()) {
						THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
							ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
							tok.to_string(false)
							);
					}
					tok = get_token(statement.at(child_position).get_id());

					if(tok.get_type() != TOKEN_OPERAND) {
						THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
							ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
							tok.to_string(false)
							);
					}

					switch(tok.get_mode()) {
						case ADDRESSING_MODE_REGISTER:
						case ADDRESSING_MODE_INDIRECT_REGISTER:
						case ADDRESSING_MODE_PUSH_POP:
						case ADDRESSING_MODE_PEEK:
						case ADDRESSING_MODE_SP_LITERAL:
						case ADDRESSING_MODE_PC_LITERAL:
						case ADDRESSING_MODE_EX_LITERAL:
						case ADDRESSING_MODE_LITERAL:
							break;
						case ADDRESSING_MODE_INDIRECT_REGISTER_NEXT_WORD:
						case ADDRESSING_MODE_PICK:
						case ADDRESSING_MODE_INDIRECT_NEXT_WORD:
						case ADDRESSING_MODE_NEXT_WORD_LITERAL:

							if(tok.get_mode() == ADDRESSING_MODE_NEXT_WORD_LITERAL) {
								value_node = statement.at(child_position);
								
								if(!value_node.size()) {
									THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
										ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
										tok.to_string(false)
										);
								}
								value_tok = get_token(statement.at(value_node.get_child_position(0)).get_id());

								if(value_tok.get_type() != TOKEN_LABEL) {
									value = _evaluate_value(value_tok, false);
								
									if(value.front() == MIN_LITERAL
											|| value.front() <= MAX_LITERAL) {
										get_token(statement.at(child_position).get_id()).set_mode(ADDRESSING_MODE_LITERAL);
									} else {
										++offset;
									}
								} else {
									++offset;
								}
							} else {
								++offset;
							}
							break;
						default:
							THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
								ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
								tok.to_string(false)
								);
					}
				}
				break;
			case TOKEN_DIRECTIVE:

				if(!statement.front().has_children()) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
						tok.to_string(false)
						);
				}
				value_list_position = statement.front().get_child_position(0);

				for(i = 0; i < statement.at(value_list_position).size(); ++i) {
					child_position = statement.at(value_list_position).get_child_position(i);

					if(child_position >= statement.size()) {
						THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
							ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
							tok.to_string(false)
							);
					}
					tok = get_token(statement.at(child_position).get_id());

					if(tok.get_type() == TOKEN_LABEL) {
						++offset;
					} else {

						switch(tok.get_subtype()) {
							case VALUE_HEXIDECIMAL:
							case VALUE_INTEGER:
								++offset;
								break;
							case VALUE_STRING_VAR:
								offset += (word_t) tok.get_text().size();
								break;
							default:
								THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
									ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
									tok.to_string(false)
									);
						}
					}
				}
				break;
			case TOKEN_LABEL:
				label_iter = _label_offset.find(tok.get_text());

				if(label_iter != _label_offset.end()) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_LABEL_REDEFINITION,
						tok.to_string(false)
						);
				}
				_label_offset.insert(std::pair<std::string, word_t>(tok.get_text(), offset));
				break;
			default:
				THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
					ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
					tok.to_string(false)
					);
		}
		move_next_statement();
	}
}

void 
_assembler::_generate_pass_1(void)
{
	token tok;
	std::vector<node> statement;
	std::vector<word_t> result, oper_a, oper_b;
	size_t i, child_position, value_list_position;
	std::map<std::string, word_t>::iterator label_iter;

	parser::reset();
	_binary_file.clear();

	while(has_next_statement()) {
		statement = get_statement();
		tok = get_token(statement.front().get_id());

		switch(tok.get_type()) {
			case TOKEN_BEGIN:
			case TOKEN_LABEL:
				break;
			case TOKEN_BASIC_OPCODE:

				if(statement.front().size() != OPCODE_BASIC_OPERAND_COUNT) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
						tok.to_string(false)
						);
				}
				oper_b = _evaluate_operand(statement, statement.front().get_child_position(0));
				oper_a = _evaluate_operand(statement, statement.front().get_child_position(1));
				_binary_file.push_back(DEFINE_OPCODE_BASIC(OPCODE_BASIC_VALUE(tok.get_subtype()), oper_a.front(), oper_b.front()));

				if(oper_a.size() > 1) {
					_binary_file.push_back(oper_a.back());
				}

				if(oper_b.size() > 1) {
					_binary_file.push_back(oper_b.back());
				}
				break;
			case TOKEN_DIRECTIVE:

				if(!statement.front().has_children()) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
						tok.to_string(false)
						);
				}
				value_list_position = statement.front().get_child_position(0);

				for(i = 0; i < statement.at(value_list_position).size(); ++i) {
					child_position = statement.at(value_list_position).get_child_position(i);

					if(child_position >= statement.size()) {
						THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
							ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
							tok.to_string(false)
							);
					}
					tok = get_token(statement.at(child_position).get_id());

					if(tok.get_type() == TOKEN_LABEL) {
						label_iter = _label_offset.find(tok.get_text());

						if(label_iter == _label_offset.end()) {
							THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
								ASSEMBLER_EXCEPTION_LABEL_UNDEFINED,
								tok.to_string(false)
								);
						}
						_binary_file.push_back(label_iter->second);
					} else {

						switch(tok.get_subtype()) {
							case VALUE_HEXIDECIMAL:
							case VALUE_INTEGER:
							case VALUE_STRING_VAR:
								result = _evaluate_value(tok, false);
								_binary_file.insert(_binary_file.end(), result.begin(), result.end());
								break;
							default:
								THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
									ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
									tok.to_string(false)
									);
						}
					}
				}
				break;
			case TOKEN_SPECIAL_OPCODE:

				if(statement.front().size() != OPCODE_SPECIAL_OPERAND_COUNT) {
					THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
						ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
						tok.to_string(false)
						);
				}
				oper_a = _evaluate_operand(statement, statement.front().get_child_position(0));
				_binary_file.push_back(DEFINE_OPCODE_SPECIAL(OPCODE_SPECIAL_VALUE(tok.get_subtype()), oper_a.front()));

				if(oper_a.size() > 1) {
					_binary_file.push_back(oper_a.back());
				}
				break;
			default:
				THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
					ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
					tok.to_string(false)
					);
		}
		move_next_statement();
	}
}

void 
_assembler::_write_file(
	const std::string &output
	)
{
	size_t ext_offset;
	std::vector<word_t> bin_data;
	std::string output_path = output;
	std::vector<word_t>::iterator word_iter;

	if(!output_path.empty()) {
		ext_offset = output_path.find_last_of(BINARY_FILE_EXTENSION[0]);

		if(ext_offset == std::string::npos
				|| output_path.substr(ext_offset, output_path.size() - ext_offset) != BINARY_FILE_EXTENSION) {
			output_path.append(BINARY_FILE_EXTENSION);
		}
		bin_data = get_binary_data();
		word_iter = bin_data.begin();
		std::ofstream file(output_path.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

		if(!file) {
			THROW_ASSEMBLER_EXCEPTION_WITH_MESSAGE(
				ASSEMBLER_EXCEPTION_WRITE_FILE_FAILED,
				"\'" << output_path << "\'"
				);
		}
		
		for(; word_iter != bin_data.end(); ++word_iter) {
			file << (byte_t) (*word_iter >> BYTE_WIDTH);
			file << (byte_t) *word_iter;
		}
		file.close();
	}
}

void 
_assembler::clear(void)
{
	LOCK_OBJECT(std::recursive_mutex, _assembler_lock);

	parser::reset();
	_binary_file.clear();
	_label_offset.clear();
	_value.clear();
}

void 
_assembler::generate(
	const std::string &output,
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, _assembler_lock);

	try {
		
		if(verbose) {
			std::cout << "Building source file(s)... pass 1... ";
		}
		_generate_pass_0();

		if(verbose) {
			std::cout << "pass 2... ";
		}
		_generate_pass_1();

		if(verbose) {
			std::cout << "Done." << std::endl;
		}

		if(!output.empty()) {

			if(verbose) {
				std::cout << "Writing to file... ";
			}
			_write_file(output);
		
			if(verbose) {
				std::cout << "Done." << std::endl;
			}
		}
	} catch(std::runtime_error &exc) {
		std::cout << "ABORTED!" << std::endl;
		throw exc;
	}
}

std::vector<word_t> &
_assembler::get_binary_data(void)
{
	LOCK_OBJECT(std::recursive_mutex, _assembler_lock);

	return _binary_file;
}

bool 
_assembler::has_binary_data(void)
{
	LOCK_OBJECT(std::recursive_mutex, _assembler_lock);

	return !_binary_file.empty();
}

void 
_assembler::initialize(
	const std::string &input,
	bool is_file
	)
{
	LOCK_OBJECT(std::recursive_mutex, _assembler_lock);

	parser::initialize(input, is_file);
	clear();
}

void 
_assembler::initialize(
	const _assembler &other
	)
{
	LOCK_OBJECT(std::recursive_mutex, _assembler_lock);

	parser::initialize(other);
	_binary_file = other._binary_file;
	_label_offset = other._label_offset;
	_value = other._value;
}

size_t 
_assembler::size(void)
{
	LOCK_OBJECT(std::recursive_mutex, _assembler_lock);

	return _binary_file.size() * sizeof(word_t);
}

std::string 
_assembler::to_string(
	bool verbose
	)
{
	LOCK_OBJECT(std::recursive_mutex, _assembler_lock);

	word_t i = 0;
	std::stringstream ss;

	if(verbose) {
		for(; i < _binary_file.size(); ++i) {
			
			if(!(i % BINARY_BLOCK_WIDTH)) {

				if(i) {
					ss << std::endl;
				}
				ss << VALUE_AS_HEX(word_t, i) << " |";
			}
			ss << " " << VALUE_AS_HEX(word_t, _binary_file.at(i));
		}
	}

	return ss.str();
}