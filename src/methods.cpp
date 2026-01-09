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
        unsigned int dummy = 0;
        string dummy_highlight = "";
        glimpse_inside_of_directory(win,filepath,dummy_highlight,dummy,dummy,dummy,false);
    }
    else
        glimpse_inside_of_text_file(win,filepath);

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
            if (!line_printable.empty() && line_printable.length()<line.length())
            {
                line_printable.back() = '\\';
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

void glimpse_inside_of_directory(WINDOW* win,string& filepath,
                                string &highlighted_file_path,
                                unsigned int& total_number_of_files,
                                unsigned int highlighted_file,
                                unsigned int starting_at,
                                bool is_in_navigation_mode)
{
    //mvwprintw(win,1,1,"This is a directory");
    unsigned int  max_height, max_width;
    getmaxyx(win,max_height,max_width);
    unsigned int height = (max_height - 2),width = (max_width - 2);
    if (!is_in_navigation_mode)
        starting_at = 0;
    fs::path directory(filepath);
    auto it = fs::directory_iterator(directory,fs::directory_options::skip_permission_denied);
    try
    {
        vector<fs::path> files;
        for (const auto & entry: it)
            files.push_back(entry.path().filename());
        sort(files.begin(),files.end());
        total_number_of_files = files.size();
        for (unsigned int pointer = 0;pointer < total_number_of_files;pointer++ )
        {
            string subject_path(files[pointer].string());
            if (subject_path.length()>=width)
            {
                if (width >= 2)
                {
                    subject_path = subject_path.substr(0,width-1);
                    subject_path[subject_path.length()-1] = '\\';
                }
                else subject_path = subject_path.substr(0,width);
            }
            if (pointer >= starting_at && pointer < starting_at+height)
            {
                unsigned int display_row = pointer - starting_at + 1;
                if (pointer == highlighted_file && is_in_navigation_mode)
                {
                    highlighted_file_path = directory / files[pointer];
                    wattron(win,COLOR_PAIR(MAGENTA_PAIR)|A_BOLD|A_UNDERLINE);
                    mvwprintw(win,display_row,2,"%s",subject_path.c_str());
                    wattroff(win,COLOR_PAIR(MAGENTA_PAIR)|A_BOLD|A_UNDERLINE);
                }
                else
                {
                    fs::path full_path = directory / files[pointer];
                    if (fs::is_directory(full_path))
                    {
                        wattron(win,COLOR_PAIR(GREEN_PAIR)|A_BOLD);
                        mvwprintw(win,display_row,2,"%s",subject_path.c_str());
                        wattroff(win,COLOR_PAIR(GREEN_PAIR)|A_BOLD);
                    }
                    else if (fs::is_empty(full_path))
                    {
                        wattron(win,COLOR_PAIR(GREY_PAIR)|A_REVERSE);
                        mvwprintw(win,display_row,2,"%s",subject_path.c_str());
                        wattroff(win,COLOR_PAIR(GREY_PAIR)|A_REVERSE);
                    }
                    else
                        mvwprintw(win,display_row,2,"%s",subject_path.c_str());
                }
            }
        }

    }
    catch (const fs::filesystem_error& e)
    {
        wattron(win,COLOR_PAIR(RED_PAIR));
        mvwprintw(win, 1,2,"Error in reading directory.");
        wattroff(win,COLOR_PAIR(RED_PAIR));
        wrefresh(win);
    }
}


void browse_in_current_directory(WINDOW* dir,WINDOW* view,WINDOW*sweetpatch,string& filepath)
{
    // { for parent, } for child path, q for quit,enter for read or traverse
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
        highlighted_file = 0,
        starting_at = 0,
        total_number_of_files = 0;
    string highlighted_file_path,buffered_child_path("");
    bool is_focused_in_dir = true;
    bool auto_read = true;
    int c;

    fs::path current_path(filepath);
    if (fs::exists(current_path))
    {
        current_path = fs::canonical(current_path);
        filepath = current_path.string();
    }


    glimpse_inside_of_directory(dir,filepath,highlighted_file_path,total_number_of_files,highlighted_file,starting_at,true);
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
        mvwprintw(sweetpatch,0,0,"current: %s | highlighted: %s | total files: %u ",current_path.c_str(),highlighted_file_path.c_str(),total_number_of_files);
        if (highlighted_file >= total_number_of_files && total_number_of_files > 0)
            highlighted_file = total_number_of_files -1;
        if (highlighted_file < starting_at)
            starting_at = highlighted_file;
        else if (highlighted_file >= starting_at + height)
            starting_at = highlighted_file - height + 1;



        glimpse_inside_of_directory(dir,filepath,highlighted_file_path,total_number_of_files,highlighted_file,starting_at,true);



        if (auto_read && current_window_mode == window_modes::DIR_MODE)
        {
            fs::path file_to_read(current_path);

            if (fs::exists(file_to_read / highlighted_file_path))
            {
                file_to_read /= highlighted_file_path;
                string file_path_to_read(file_to_read.string());
                unsigned int dummy = 0;
                string dummy_highlight = "s";
                if (fs::is_directory(file_to_read))
                    glimpse_inside_of_directory(
                        view,
                        file_path_to_read,
                        dummy_highlight,
                        dummy,
                        dummy);
                else
                    glimpse_inside(view,file_path_to_read);
            }
        }
        wrefresh(dir);
        wrefresh(view);
        c = getch();
        switch (tolower(c))
        {
            // ----- switch modes -----
            case KEY_ESC:
                {
                    is_focused_in_dir = false;
                }
                break;
            // ----- switch modes -----
                
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
                    if (highlighted_file <= total_number_of_files-1) highlighted_file++;
                }
                break;

            case '{':
                {
                    if (current_path.has_parent_path())
                    {
                        buffered_child_path = current_path.string();
                        current_path = current_path.parent_path();
                        filepath = current_path.string();
                    }
                }
                break;
            case '}':
                {
                    if (!buffered_child_path.empty() && fs::exists(buffered_child_path))
                    {
                        current_path = fs::path (buffered_child_path);
                        if (!fs::is_directory(current_path)) current_path = current_path.parent_path();
                        filepath = current_path.string();
                        starting_at = 0;
                        highlighted_file = 0;
                    }
                }
                break;

            case 10: // newline -- read or traverse
                {
                    fs::path file_to_read(current_path / highlighted_file_path);
                    if (fs::exists(file_to_read))
                    {

                        if (fs::is_directory(file_to_read))
                        {
                            current_path = file_to_read;
                            filepath = current_path.string();
                            starting_at = 0;
                            highlighted_file = 0;
                        }
                        else
                        {
                            string file_path_to_read(file_to_read);
                            directory_mode_browse_in_current_file(view,file_path_to_read);
                        }

                    }
                }
                break;

            case 'r': // refresh
                {
                    glimpse_inside_of_directory(dir,filepath,highlighted_file_path,total_number_of_files,highlighted_file,starting_at,true);
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
        length_of_largest_visible_line = 0;
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

            if (number_of_lines >= y_starting_at &&
                 number_of_lines < y_starting_at+height)
            {
                mvwprintw(win,number_of_lines-y_starting_at+1,1,
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
                break;
            }
            break;
            case KEY_DOWN:
            {
                if (y_starting_at+(height/3)<number_of_lines) y_starting_at +=  height/3;
                break;
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
                break;
            }
            break;
            case KEY_HOME:
            {
                x_starting_at = 0;
                break;
            }
            break;
            case KEY_END:
            {
                x_starting_at = length_of_largest_visible_line-5;
                break;
            }
            break;
            default:break;
        }
        // ----- keys -----
    }
    // ----- reserved -----
    // x_starting_at = 0;
    // y_starting_at = 0;
    // number_of_lines = 0;
    // length_of_largest_line = 0;
    // length_of_largest_visible_line = 0;
    // line_buffer.clear();
    // printable_buffer.clear();
    // ----- reserved -----
}



// ----- editors -----
// . \author Joseph Wangai Mwaniki
// .
// .
// .
// .
// .
// .
// .
// .
// .
// .
// ----- editors -----

void command_mode_write_script(WINDOW* win,WINDOW* sweetpatch,string& filepath)
{}

// ----- read -----
bool load_file_to_buffer(WINDOW* win,string&buffer,string& filepath)
{
    unsigned int max_width = static_cast<unsigned int>(getmaxx(win));
    unsigned int width = max_width - 2;
    vector<string> error_msgs = {
        "File not opened. Check file permissions if file exists."
        "File not read. Check file permissions if file exists else your memory has run out."
    };
    
    ifstream file (filepath, ios::binary|ios::ate);
    if (!file.is_open())
    {
        wattron(win,COLOR_PAIR(RED_PAIR));
        string error_msg = error_msgs[0].substr(0,width-3);
        error_msg.push_back('\\');
        mvwprintw(win,0,2,"%s",error_msg);
        return false;
    }
    streamsize size = file.tellg(); // get file size
    file.seekg(0,ios::beg); // go to the begining of the file for parsing
    script_file_buffer.resize(size);
    if (!file.read(&script_file_buffer[0],size))
    {
        wattron(win,COLOR_PAIR(RED_PAIR));
        string error_msg = error_msgs[1].substr(0,width-3);
        error_msg.push_back('\\');
        mvwprintw(win,0,2,"%s",error_msg);
        return false;
    }
}

// ----- write -----
void add_char_to_buffer(string& buffer,unsigned int c)
{}

void remove_chars_from_buffer(string& buffer,bool chars_is_word)
{}

void move_cursor_at_position_y_x_in_buffer(string& buffer,
                                            unsigned int y_pos,
                                            unsigned int x_pos)
{}

void overwrite_buffer_char_at_position_y_x_in_buffer(string& buffer,
                                                    unsigned int& y_pos,
                                                    unsigned int& x_pos)
{}

void append_to_buffer_after_char_at_position_y_x_in_buffer(string&buffer,
                                                            unsigned int& y_pos,
                                                            unsigned int& x_pos,
                                                            unsigned int c)
{}

// ----- Monitor changes -----
// ----- Backup & Recovery -----
void file_write_crash_log(string& buffer,string& filename)
{}

void file_write_update_file_cache(string& filename)
{}

void file_write_recover_crashed_from_cache(string& filename)
{}

// ----- handle different encodings -----
// ----- add line endings -----
// ---- lock down subject file -----
void file_write_update_file_journal(string& filename)
{}
// ----- autosave after an amount of time -----
// impliment later
