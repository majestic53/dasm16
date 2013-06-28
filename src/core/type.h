/*
 * type.h
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

#ifndef TYPE_H_
#define TYPE_H_

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>

#define BYTE_WIDTH (8)
#define INVALID "INVALID"
#define INVALID_BYTE ((uint8_t) -1)
#define INVALID_TYPE ((size_t) -1)
#define INVALID_WORD ((uint16_t) -1)
#define INVALID_VALUE (-1)
#define SHOW_GLOBAL_EXCEPTION_HEADER false
#define UNKNOWN "UNKNOWN"
#define UNKNOWN_EXCEPTION "Unknown exception"

#define __TO_STRING_X(_T_) #_T_
#define LOCK_OBJECT(_T_, _M_) std::lock_guard<_T_> __LOCK_##_M_(_M_)
#define REFERENCE_PARAMETER(_P_) ((void) _P_)
#define THROW_EXCEPTION(_H_, _V_, _VB_) {\
	std::stringstream __ss;\
	bool __result = _VB_ && SHOW_GLOBAL_EXCEPTION_HEADER;\
	if(__result) { __ss << "[" << _H_ << "] "; }\
	__ss << _V_;\
	throw std::runtime_error(__ss.str().c_str());\
	}
#define THROW_EXCEPTION_WITH_MESSAGE(_H_, _V_, _M_, _VB_) {\
	std::stringstream __ss;\
	bool __result = _VB_ && SHOW_GLOBAL_EXCEPTION_HEADER;\
	if(__result) { __ss << "[" << _H_ << "] "; }\
	__ss << _V_ << ": " << _M_;\
	throw std::runtime_error(__ss.str().c_str());\
	}
#define TO_STRING(_T_) __TO_STRING_X(_T_)
#define TRANSFORM(_T_, _TR_) std::transform(_TR_.begin(), _TR_.end(), _TR_.begin(), _T_)
#define VALUE_AS_HEX(_T_, _V_)\
	std::setw(sizeof(_T_) * 2) << std::setfill('0') << std::hex << ((unsigned) _V_) << std::dec

typedef uint8_t byte_t, *byte_ptr;
typedef uint16_t word_t, *word_ptr;

#endif