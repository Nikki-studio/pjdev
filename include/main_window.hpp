/** ./main_window.hpp
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


#ifndef MAIN_WINDOW_HPP_INCLUDED
#define MAIN_WINDOW_HPP_INCLUDED

#include "methods.hpp"
#include "main.hpp"

class main_window
{
private:
    bool is_running;
    vector<string> opened_files;
    string current_file;
public:
    main_window(/* args */);
    ~main_window();
};

main_window::main_window(/* args */)
{
}

main_window::~main_window()
{
}

void init_colors();

#endif // MAIN_WINDOW_HPP_INCLUDED