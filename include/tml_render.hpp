/** ./tml_renderer.hpp
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


#ifndef TML_RENDER_HPP_INCLUDED
#define TML_RENDER_HPP_INCLUDED

#include "methods.hpp"

class tml_render
{
private:
    vector<tml_token_struct> tokens;
    WINDOW* win;
    string processed_string;

public:
    tml_render(WINDOW* win,const string& format,...);
    void render();
    ~tml_render();

};



#endif // TML_RENDER_HPP_INCLUDED