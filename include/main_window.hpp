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