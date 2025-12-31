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

 
typedef enum tml_property
{
    _none,
    _attributes,
    _normal,
    _standout,
    _underline, 
    _reverse,
    _blink,
    _dim,
    _bold,
    _altcharset,
    _invisible,
    _protect,
    _horizontal,
    _left,
    _low,
    _right,
    _top,
    _vertical
} tml_properties;

typedef enum tag_color
{
    none,
    black,
    red,
    green,
    yellow,
    blue,
    cyan,
    magenta, //purple
    white,
    // ... 
    grey,
    pink,
    maroon,
    light_blue,
    light,
    light_green,
    gold,
    brown
}tag_color;

typedef struct tml_token_struct
{
    tag_color color;
    vector<enum tml_property> tml_properties;
}tml_token_struct;


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