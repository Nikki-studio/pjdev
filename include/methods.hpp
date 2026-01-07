/** ./methods.hpp
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


#ifndef METHODS_HPP_INCLUDED
#define METHODS_HPP_INCLUDED
#include "main.hpp"
void view_when_error(WINDOW win);
void glimpse_inside(WINDOW* win,string& filepath);
void glimpse_inside_of_text_file(WINDOW* win,string& filepath);
void glimpse_inside_of_directory(WINDOW* win,string& filepath);

void browse_in_current_directory(WINDOW* dir,WINDOW* view,string& filepath);
void browse_in_current_file(WINDOW* win,string& filepath);
#endif // METHODS_HPP_INCLUDED
