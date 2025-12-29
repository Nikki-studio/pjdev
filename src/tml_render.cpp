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
    wprintw(this ->win,processed_string.c_str());
    wrefresh(this ->win);
}

tml_render::~tml_render()
{
}
