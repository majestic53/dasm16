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

#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>

#define LOCK_OBJECT(_T_, _M_) std::lock_guard<_T_> __LOCK_##_M_(_M_)
#define REFERENCE_PARAMETER(_P_) ((void) _P_)
#define SHOW_GLOBAL_EXCEPTION_HEADER false
#define UNKNOWN_EXCEPTION "Unknown exception"
#define THROW_EXCEPTION(_H_, _V_, _VB_) {\
	std::stringstream ss;\
	bool result = SHOW_GLOBAL_EXCEPTION_HEADER && _VB_;\
	if(result) { ss << "[" << _H_ << "] "; }\
	ss << _V_;\
	throw std::runtime_error(ss.str().c_str());\
	}
#define THROW_EXCEPTION_WITH_MESSAGE(_H_, _V_, _M_, _VB_) {\
	std::stringstream ss;\
	bool result = SHOW_GLOBAL_EXCEPTION_HEADER && _VB_;\
	if(result) { ss << "[" << _H_ << "] "; }\
	ss << _V_ << ": " << _M_;\
	throw std::runtime_error(ss.str().c_str());\
	}

#endif