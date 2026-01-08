/** ./main.hpp
 *  Copyright (C) 2024  Joseph Wangai Mwaniki joewamwaniki@gmail.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#define AUTHOR  "\"Joseph Wangai Mwaniki\""
#define PROGRAM "\"pjdev\""
#define PROJECT "\"project JOB\""

#include <filesystem>
#include <iostream>
#include <vector>
#include <ncursesw/ncurses.h>
#include <fstream>
#include <cstdarg>
#include <cstdio>
#include <cmath>

using namespace std;
namespace fs = filesystem;



// Color definition
#define BLACK_PAIR     1
#define RED_PAIR       2
#define GREEN_PAIR     3
#define YELLOW_PAIR    4
#define BLUE_PAIR      5
#define CYAN_PAIR      6
#define MAGENTA_PAIR   7
#define WHITE_PAIR     8
#define GREY_PAIR      9
#define PINK_PAIR      10
#define MAROON_PAIR    11
#define LIGHT_BLUE_PAIR 12
#define LIGHT_PAIR     13
#define LIGHT_GREEN_PAIR 14
#define GOLD_PAIR      15
#define BROWN_PAIR     16


static string subject_file_buffer, script_file_buffer;

#endif // MAIN_HPP_INCLUDED