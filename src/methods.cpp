/** ./methods.cpp
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


#include "methods.hpp"

void view_when_error(WINDOW* win)
{
    unsigned int max_height, max_width;
    getmaxyx(win,max_height,max_width);
    for (unsigned int y = 1; y < max_height-1;y++)
            {
                for (unsigned int i = 1; i < max_width-1; i++)
                {
                    mvwaddch(win,y,i,'x');
                }
            }
}

void glimpse_inside(WINDOW* win,string filepath)
{
    fs::path inode(filepath);
    wprintw(win,"here");
    box(win,0,0);
    if (!fs::exists(inode))
    {
        view_when_error(win);
    }

    if (fs::is_directory(inode))
    {
        glimpse_inside_of_directory(win,filepath);
        return;
    }
    else
    {
        glimpse_inside_of_text_file(win,filepath);
        return;
    }

    wrefresh(win);
}

void glimpse_inside_of_text_file(WINDOW* win,string filepath)
{
    unsigned int max_height, max_width;
    getmaxyx(win,max_height,max_width);
    ifstream text_file(filepath);
    bool file_is_printable = true;
    string line;
    file_is_printable = (text_file.is_open())? true :false;
    
    if (file_is_printable)
    {
        unsigned int line_no = 1;
        while(getline(text_file,line))
        {
            string line_printable = line.substr(0,(size_t)(max_width-2));
            if (line_printable.length()<line.length())
            {
                line_printable[line_printable.length()-1] = '\\';
            }
            mvwprintw(win,line_no,1,"%s",line_printable.c_str());
            if (line_no>=max_height-2)
                break;
            line_no++;
        }
    }
    else if (!file_is_printable)
    {
        view_when_error(win);
    }
    else
    {
        cerr << "unknown error at" << __func__<<"\n";
        exit(1);
    }
    text_file.close();
}

void glimpse_inside_of_directory(WINDOW* win,string filepath)
{
    //mvwprintw(win,1,1,"This is a directory");
    unsigned int  max_height, max_width,height = 1,width;
    getmaxyx(win,max_height,max_width);
    fs::path directory(filepath);
    auto it = fs::directory_iterator(directory,fs::directory_options::none);
    for (const auto & entry: it)
    {
        mvwprintw(win,height,2,"%s",entry.path().filename().c_str());
        if (height>=max_height-2)
            return;
        height++;
    }
}
