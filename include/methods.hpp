/** ./methods.hpp
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


#ifndef METHODS_HPP_INCLUDED
#define METHODS_HPP_INCLUDED
#include "main.hpp"
void view_when_error(WINDOW win);
void glimpse_inside (WINDOW* win,string& filepath);
void glimpse_inside_of_text_file(WINDOW* win,string& filepath);
unsigned int glimpse_inside_of_directory(WINDOW* win,string& filepath, string& highlighted_file_path,unsigned int higlighted_file = 0,unsigned int starting_at = 0,bool is_in_navigation_mode = false);

void browse_in_current_directory(WINDOW* dir,WINDOW* view,WINDOW*sweetpatch,string& filepath);
void directory_mode_browse_in_current_file(WINDOW* win,string& filepath);
void directory_mode_browse_in_current_file(WINDOW* win,string& filepath);

void command_mode_write_script(WINDOW* win,string& filepath);
// ----- read -----
void load_file_to_buffer(string&buffer,string& filepath);
// ----- write -----
void add_char_to_buffer(string& buffer,unsigned int c);
void remove_chars_from_buffer(string& buffer,bool chars_is_word);
void move_cursor_at_position_y_x_in_buffer(string& buffer,unsigned int y_pos,unsigned int x_pos);
void overwrite_buffer_char_at_position_y_x_in_buffer(string& buffer,unsigned int& y_pos,unsigned int& x_pos);
void append_to_buffer_after_char_at_position_y_x_in_buffer(string&buffer,unsigned int& y_pos,unsigned int& x_pos, unsigned int c);
// ----- Monitor changes -----
// ----- Backup & Recovery -----
void file_write_crash_log(string& buffer,string& filename);
void file_write_update_file_cache(string& filename);
void file_write_recover_crashed_from_cache(string& filename);
// ----- handle different encodings -----
// ----- add line endings -----
// ---- lock down subject file -----
void file_write_update_file_journal(string& filename);
// ----- autosave after an amount of time -----
// impliment later
#endif // METHODS_HPP_INCLUDED
