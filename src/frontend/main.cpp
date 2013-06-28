/*
 * main.cpp
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

#include <ctime>
#include <iostream>
#include "dasm16.h"

enum {
	NO_ERROR = 0,
	ASM_ERROR,
	INP_ERROR,
	DISP_HELP,
	DISP_USAGE,
};

#define APP_TITLE "DASM16"
#define HELP_FLAG 'h'
#define OUTPUT_FLAG 'o'
#define MSEC_TO_SEC (1000.0)
#define VERBOSE_FLAG 'v'
#define USAGE_STRING "Usage: dasm16 [-h | -v] [-o OUTPUT] input..."

int
main(
	int argc,
	char **argv
	)
{
	time_t build_time;
	bool verbose_mode = false;
	int i = 1, result = NO_ERROR;
	std::string in_file, out_file, flag;

	if(argc < 2) {
		std::cerr << APP_TITLE << " " << dasm16::version(true) << std::endl 
			<< USAGE_STRING << std::endl;
		result = DISP_USAGE;
	} else {

		for(; i < argc; ++i) {
			
			if(*argv[i] == '-'
					|| *argv[i] == '/') {
				flag = argv[i];

				if(flag.size() > 2) {
					std::cerr << "Unknown flag: \'-" << flag << "\'" << std::endl;
					result = INP_ERROR;
					break;
				}

				switch(flag.at(1)) {
					case HELP_FLAG:
						std::cout << APP_TITLE << " " << dasm16::version(true) << std::endl
							<< USAGE_STRING << std::endl << std::endl
							<< "-" << HELP_FLAG << ": Display help message" << std::endl
							<< "-" << OUTPUT_FLAG << ": Set output file path" << std::endl
							<< "-" << VERBOSE_FLAG << ": Set verbose mode" << std::endl;
						result = DISP_HELP;
						break;
					case OUTPUT_FLAG:

						if(i == (argc - 1)) {
							std::cerr << "Missing flag input: \'-" << OUTPUT_FLAG << "\'" << std::endl;
							result = INP_ERROR;
						} else {
							out_file = argv[++i];
						}
						break;
					case VERBOSE_FLAG:
						verbose_mode = true;
						break;
					default:
						std::cerr << "Unknown flag: \'-" << flag << "\'" << std::endl;
						result = INP_ERROR;
						break;
				}

				if(result) {
					break;
				}
			} else {
				in_file = argv[i];
			}
		}

		if(!result
				&& in_file.empty()) {
			std::cerr << "Missing input file parameter" << std::endl;
			result = INP_ERROR;
		}

		if(!result
				&& out_file.empty()) {
			std::cerr << "Missing output file parameter" << std::endl;
			result = INP_ERROR;
		}

		if(!result) {
			build_time = clock();

			if(verbose_mode) {
				std::cout << APP_TITLE << " " << dasm16::version(true) << std::endl << "Initializing... ";
			}
			dasm16::initialize();

			if(verbose_mode) {
				std::cout << "Done." << std::endl;
			}

			try {
				dasm16::invoke_assembler(in_file, out_file, true, verbose_mode);
			} catch(std::runtime_error &exc) {
				std::cerr << "EXCEPTION: " << exc.what() << std::endl;
				result = ASM_ERROR;
			}

			if(verbose_mode) {
				std::cout << (result == NO_ERROR ? "Build successful. (" : "BUILD FAILED! (") 
						<< ((clock() - build_time) / MSEC_TO_SEC) << " sec.)" << std::endl;
			}
		}
	}

	return result;
}