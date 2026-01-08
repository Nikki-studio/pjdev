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

void glimpse_inside(WINDOW* win,string& filepath)
{
    fs::path inode(filepath);
    wprintw(win,"here");
    box(win,0,0);
    if (!fs::exists(inode))
    {
        wattron(win,COLOR_PAIR(RED_PAIR));
        view_when_error(win);
        mvwprintw(win,0,3,"%s",filepath.c_str());
        wattroff(win,COLOR_PAIR(RED_PAIR));
        wrefresh(win);
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

void glimpse_inside_of_text_file(WINDOW* win,string& filepath)
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

void glimpse_inside_of_directory(WINDOW* win,string& filepath)
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

void directory_mode_browse_in_current_directory(WINDOW* dir,WINDOW* view,string& filepath)
{
    // p for parent, q for quit,enter for read or traverse
    // a for autoread toggle, r for new refresh
    // arrowkeys for dir navigation
    // ----- reserved -----
    curs_set(0);
    box(dir,0,0);
    box(view,0,0);
    unsigned int  max_height, max_width;
    getmaxyx(dir,max_height,max_width);
    unsigned int
        height = max_height -2,
        width = max_width -2,
        highlighted_file = 3,
        starting_at = 0,
        max_displayable = height;

    bool is_focused_in_dir = true;
    bool auto_read = true;
    int c;
    vector<string> filepaths;
    vector<bool> is_directory;

    directories:
    fs::path current_path(filepath);
    if (!fs::is_directory(current_path))
        current_path = fs::canonical(current_path.parent_path());

        
    filepath = current_path.string();
    filepath = current_path.string();

    if (current_path.has_parent_path() && current_path != current_path.root_path())
    {
        filepaths.push_back("..");
        is_directory.push_back(true);
    }

    try
    {
        for (const auto& entry:fs::
            directory_iterator(current_path,
                fs::directory_options::skip_permission_denied))
        {
            filepaths.push_back(entry.path().filename().string());
            is_directory.push_back(entry.is_directory());
        }
    }
    catch (const fs::filesystem_error& e)
    {
        wattron(dir,COLOR_PAIR(RED_PAIR));
        mvwprintw(dir, 1,2,"Error in reading directory.");
        wattroff(dir,COLOR_PAIR(RED_PAIR));
        wrefresh(dir);
    }
    int total_files = filepaths.size();
    // ----- reserved -----
    while (is_focused_in_dir)
    {
        // ----- keycheck -----
        werase(dir);
        werase(view);
        box(dir,0,0);
        box(view,0,0);
        if (highlighted_file >= total_files && total_files > 0)
            highlighted_file = total_files -1;
        if (highlighted_file < starting_at)
            starting_at = highlighted_file;
        else if (highlighted_file >= starting_at + max_displayable)
            starting_at = highlighted_file - max_displayable + 1;
        int display_index = 0;
        for (size_t i = starting_at;
            i < filepaths.size() && display_index < max_displayable;
            i++,display_index++)
        {
            int y_pos = display_index + 1;

            if (i == highlighted_file)
            {
                wattron(dir, COLOR_PAIR(MAGENTA_PAIR) | A_BOLD );
                mvwprintw(dir,y_pos,2,"%s",filepaths[i].c_str());
                wattroff(dir, COLOR_PAIR(MAGENTA_PAIR) | A_BOLD);
            }
            else
            {
                if (is_directory[i])
                {
                    wattron(dir, COLOR_PAIR(GREEN_PAIR) );
                    mvwprintw(dir,y_pos,2,"%s",filepaths[i].c_str());
                    wattroff(dir, COLOR_PAIR(GREEN_PAIR));
                }
                else
                    mvwprintw(dir,y_pos,2,"%s",filepaths[i].c_str());
            }
        }
        if (auto_read)
        {
            fs::path file_to_read(current_path);
            if (filepaths[highlighted_file]=="..")
                file_to_read = file_to_read.parent_path();
            else file_to_read /= filepaths[highlighted_file];
            if (fs::exists(file_to_read) && !fs::is_directory(file_to_read))
            {
                string file_path_to_read(file_to_read.string());
                glimpse_inside(view,file_path_to_read);
            }
        }
        wrefresh(dir);
        wrefresh(view);
        c = getch();
        switch (tolower(c))
        {
            case 'q':
                {
                    is_focused_in_dir = false;
                }
                break;

                // ----- arrow keys -----
            case KEY_LEFT:
            case KEY_UP:
                {
                    if (highlighted_file > 0) highlighted_file--;
                }
                break;

            case KEY_RIGHT:
            case KEY_DOWN:
                {
                    if (highlighted_file <= filepaths.size()-1) highlighted_file++;
                }
                break;

            case 'p':
                {
                    if (current_path.has_parent_path())
                    {
                        current_path = current_path.parent_path();
                        filepath = current_path.string();
                        filepaths.clear();
                        is_directory.clear();
                        goto directories;
                    }
                }
                break;

            case 10: // newline -- read or traverse
                {
                    if (filepaths[highlighted_file]=="..")
                        current_path = current_path.parent_path();
                    else current_path /= filepaths[highlighted_file];
                    filepath = current_path.string();
                    if (!is_directory[highlighted_file])
                    {
                        auto_read = false;
                        directory_mode_browse_in_current_file(view,filepath);
                        break;
                    }
                }

            case 'r': // refresh
                {
                    if (!filepaths.empty()) filepaths.clear();
                    if (!is_directory.empty()) is_directory.clear();
                        goto directories;
                }
                break;

            case 'a': // toggle autoread on or off
                {
                    auto_read = !auto_read;
                }
                break;

            default:
                break;
        }
        // ----- keycheck -----
    }
    curs_set(1);
}

void directory_mode_browse_in_current_file(WINDOW* win,string& filepath)
{
    // q for quit
    // i,j,k,l for view navigation
    // ----- reserved -----
    bool is_navigating = true;
    unsigned int
        max_width,
        max_height;
    getmaxyx(win,max_height,max_width);
    unsigned int
        height = max_height -2,
        width = max_width - 2,
        x_starting_at = 0,
        y_starting_at = 0,
        number_of_lines,
        length_of_largest_line,
        length_of_largest_visible_line;
    string line_buffer,printable_buffer;
    int c;
    // ----- reserved -----
    while (is_navigating)
    {
        c = getch();

        length_of_largest_line = 0;
        number_of_lines =0;
        werase(win);
        box(win,0,0);
        ifstream file_to_read(filepath);
        if (!file_to_read.is_open())
        {
            wattron(win,COLOR_PAIR(RED_PAIR));
            mvwprintw(win,1,1,"%s",filepath.c_str());
            wattroff(win,COLOR_PAIR(RED_PAIR));
            wrefresh (win);
            is_navigating = false;
        }
        while (getline(file_to_read,line_buffer))
        {
            if (length_of_largest_line<line_buffer.length())
                length_of_largest_line = line_buffer.length();

            if (line_buffer.length() <=x_starting_at)
            {
                printable_buffer = "";
            }
            else if (line_buffer.length() <= x_starting_at+width)
            {
                printable_buffer = line_buffer.substr(x_starting_at);
            }
            else
            {
                printable_buffer = line_buffer.substr(x_starting_at,width);
            }


            if (x_starting_at > 0 && !printable_buffer.empty())
                printable_buffer[0] = '/';

            if (x_starting_at+width < line_buffer.length() &&
                 !printable_buffer.empty())
                printable_buffer[printable_buffer.length()-1] = '\\';

            if (number_of_lines > y_starting_at &&
                 number_of_lines < y_starting_at+height)
            {
                mvwprintw(win,number_of_lines-y_starting_at,1,
                    "%s",printable_buffer.c_str());
                if (length_of_largest_line>length_of_largest_visible_line)
                    length_of_largest_line = length_of_largest_visible_line;
            }
            number_of_lines++;
        }
        file_to_read.close();
        wrefresh(win);
        // ----- keys -----
        switch (tolower(c))
        {
            // ----- vertical -----
            case 'i':
            {
                if (y_starting_at>0)
                    y_starting_at--;
            }
            break;
            case 'k':
            {
                if (y_starting_at+height<number_of_lines)
                    y_starting_at++;
            }
            break;
            case KEY_UP:
            {
                if (y_starting_at > height) y_starting_at -= height/3;
                return;
            }
            break;
            case KEY_DOWN:
            {
                if (y_starting_at+(height/3)<number_of_lines) y_starting_at +=  height/3;
                return;
            }
            break;
            // ----- vertical -----

            // ----- horizontal -----
            case 'j':
            {
                if (x_starting_at>0)
                    x_starting_at--;
            }
            break;
            case 'l':
            {
                //if (x_starting_at+width<length_of_largest_visible_line)
                    x_starting_at++;
            }
            break;
            // ----- horizontal -----
            case 'q':
            {
                is_navigating = false;
                return;
            }
            break;
            case KEY_HOME:
            {
                x_starting_at = 0;
                return;
            }
            break;
            case KEY_END:
            {
                x_starting_at = length_of_largest_visible_line-5;
                return;
            }
            break;
            default:break;
        }
        // ----- keys -----
    }
    // ----- reserved -----
    x_starting_at = 0;
    y_starting_at = 0;
    number_of_lines = 0;
    length_of_largest_line = 0;
    length_of_largest_visible_line = 0;
    line_buffer.clear();
    printable_buffer.clear();
    // ----- reserved -----
}


// ----- editors -----
// \author Joseph Wangai Mwaniki
// journal file
// string buffer
// 
// 
// 
// 
// 
// 
// 
// 
// 


void command_mode_browse_in_current_directory(WINDOW* dir,WINDOW* view,WINDOW*sweetpatch,string& filepath)
{
    // p for parent, q for quit,enter for read or traverse
    // a for autoread toggle, r for new refresh
    // arrowkeys for dir navigation
    // ----- reserved -----
    curs_set(0);
    box(dir,0,0);
    box(view,0,0);
    box(sweetpatch,0,0);
    unsigned int  max_height, max_width;
    getmaxyx(dir,max_height,max_width);
    unsigned int
        height = max_height -2,
        width = max_width -2,
        highlighted_file = 3,
        starting_at = 0,
        max_displayable = height;

    bool is_focused_in_dir = true;
    bool auto_read = true;
    int c;
    vector<string> filepaths;
    vector<bool> is_directory;

    directories:
    fs::path current_path(filepath);
    if (!fs::is_directory(current_path))
        current_path = fs::canonical(current_path.parent_path());

        
    filepath = current_path.string();
    filepath = current_path.string();

    if (current_path.has_parent_path() && current_path != current_path.root_path())
    {
        filepaths.push_back("..");
        is_directory.push_back(true);
    }

    try
    {
        for (const auto& entry:fs::
            directory_iterator(current_path,
                fs::directory_options::skip_permission_denied))
        {
            filepaths.push_back(entry.path().filename().string());
            is_directory.push_back(entry.is_directory());
        }
    }
    catch (const fs::filesystem_error& e)
    {
        wattron(dir,COLOR_PAIR(RED_PAIR));
        mvwprintw(dir, 1,2,"Error in reading directory.");
        wattroff(dir,COLOR_PAIR(RED_PAIR));
        wrefresh(dir);
    }
    int total_files = filepaths.size();
    // ----- reserved -----
    while (is_focused_in_dir)
    {
        // ----- keycheck -----
        werase(dir);
        werase(view);
        werase(sweetpatch);
        box(dir,0,0);
        box(view,0,0);
        box(sweetpatch,0,0);
        if (highlighted_file >= total_files && total_files > 0)
            highlighted_file = total_files -1;
        if (highlighted_file < starting_at)
            starting_at = highlighted_file;
        else if (highlighted_file >= starting_at + max_displayable)
            starting_at = highlighted_file - max_displayable + 1;
        int display_index = 0;
        for (size_t i = starting_at;
            i < filepaths.size() && display_index < max_displayable;
            i++,display_index++)
        {
            int y_pos = display_index + 1;

            if (i == highlighted_file)
            {
                wattron(dir, COLOR_PAIR(MAGENTA_PAIR) | A_BOLD );
                mvwprintw(dir,y_pos,2,"%s",filepaths[i].c_str());
                wattroff(dir, COLOR_PAIR(MAGENTA_PAIR) | A_BOLD);
            }
            else
            {
                if (is_directory[i])
                {
                    wattron(dir, COLOR_PAIR(GREEN_PAIR) );
                    mvwprintw(dir,y_pos,2,"%s",filepaths[i].c_str());
                    wattroff(dir, COLOR_PAIR(GREEN_PAIR));
                }
                else
                    mvwprintw(dir,y_pos,2,"%s",filepaths[i].c_str());
            }
        }
        if (auto_read)
        {
            fs::path file_to_read(current_path);
            if (fs::exists(file_to_read / filepaths[highlighted_file]))
            {
                file_to_read /= filepaths[highlighted_file];
                string file_path_to_read(file_to_read.string());
                glimpse_inside(view,file_path_to_read);
                file_path_to_read.clear();
            }
            file_to_read.clear();
        }
        wrefresh(dir);
        wrefresh(view);
        wrefresh(sweetpatch);
        c = getch();
        switch (tolower(c))
        {
            case 'q':
                {
                    is_focused_in_dir = false;
                }
                break;

                // ----- arrow keys -----
            case KEY_LEFT:
            case KEY_UP:
                {
                    if (highlighted_file > 0) highlighted_file--;
                }
                break;

            case KEY_RIGHT:
            case KEY_DOWN:
                {
                    if (highlighted_file <= filepaths.size()-1) highlighted_file++;
                }
                break;

            case 'p':
                {
                    if (current_path.has_parent_path())
                    {
                        current_path = current_path.parent_path();
                        filepath = current_path.string();
                        filepaths.clear();
                        is_directory.clear();
                        goto directories;
                    }
                }
                break;

            case 10: // newline -- read or traverse
                {
                    if (filepaths[highlighted_file]=="..")
                        current_path = current_path.parent_path();
                    else current_path /= filepaths[highlighted_file];
                    filepath = current_path.string();
                    if (!is_directory[highlighted_file])
                    {
                        auto_read = false;
                        directory_mode_browse_in_current_file(view,filepath);
                        break;
                    }
                }

            case 'r': // refresh
                {
                    if (!filepaths.empty()) filepaths.clear();
                    if (!is_directory.empty()) is_directory.clear();
                        goto directories;
                }
                break;

            case 'a': // toggle autoread on or off
                {
                    auto_read = !auto_read;
                }
                break;

            default:
                break;
        }
        // ----- keycheck -----
    }
    curs_set(1);
}
