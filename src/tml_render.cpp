/** ./tml_render.cpp
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



#include "tml_render.hpp"

tml_render::tml_render(WINDOW* win,const string& format,...)
{
    this ->win = win;
    va_list args;
    va_start(args,format);

    va_list args_copy;
    va_copy(args_copy,args);

    int size = vsnprintf(nullptr,0,format.c_str(),args_copy);
    va_end(args_copy);

    if (size>0)
    {
        vector<char> buffer(size+1);
        vsnprintf(buffer.data(),size+1,format.c_str(),args);
        this -> processed_string = string(buffer.data());
    }
    va_end(args);
}

void tml_render::render()
{
    //wprintw(this ->win,processed_string.c_str());
    wrefresh(this ->win);
}

tml_render::~tml_render()
{
}
