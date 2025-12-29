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