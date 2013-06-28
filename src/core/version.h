/*
 * version.h
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

#ifndef VERSION_H_
#define VERSION_H_

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_YRWW 1326
#define VERSION_WEEK_REV 7

#define __TO_STRING_X(_T_) #_T_
#define TO_STRING(_T_) __TO_STRING_X(_T_)
#define VERSION_STRING\
	VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_YRWW
#define VERSION_STRING_VERBOSE\
	VERSION_STRING << " (rev. " << VERSION_WEEK_REV << ", " << __DATE__ << ", " << __TIME__ << ")"
#define VERSION_RES_STRING\
	TO_STRING(VERSION_MAJOR) "." TO_STRING(VERSION_MINOR) "\0"
#define VERSION_RES_STRING_VERBOSE\
	TO_STRING(VERSION_MAJOR) "." TO_STRING(VERSION_MINOR) "." TO_STRING(VERSION_YRWW) TO_STRING(VERSION_WEEK_REV) "\0"

#endif
