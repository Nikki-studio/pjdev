/** main.cpp
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

void init_colors()
{
    // First 8 standard colors
    init_pair(BLACK_PAIR, COLOR_BLACK, COLOR_BLACK);
    init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(CYAN_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(MAGENTA_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(WHITE_PAIR, COLOR_WHITE, COLOR_BLACK);

    // For extended colors, we need to check terminal support
    if (can_change_color() && COLORS >= 16) {
        // Define custom colors (RGB values 0-1000)
        init_color(8, 500, 500, 500);     // Grey
        init_color(9, 1000, 500, 500);    // Pink
        init_color(10, 500, 0, 0);        // Maroon
        init_color(11, 200, 600, 1000);   // Light Blue
        init_color(12, 800, 800, 800);    // Light
        init_color(13, 400, 1000, 400);   // Light Green
        init_color(14, 1000, 800, 0);     // Gold
        init_color(15, 600, 400, 200);    // Brown

        // Create pairs for custom colors
        init_pair(GREY_PAIR, 8, COLOR_BLACK);
        init_pair(PINK_PAIR, 9, COLOR_BLACK);
        init_pair(MAROON_PAIR, 10, COLOR_BLACK);
        init_pair(LIGHT_BLUE_PAIR, 11, COLOR_BLACK);
        init_pair(LIGHT_PAIR, 12, COLOR_BLACK);
        init_pair(LIGHT_GREEN_PAIR, 13, COLOR_BLACK);
        init_pair(GOLD_PAIR, 14, COLOR_BLACK);
        init_pair(BROWN_PAIR, 15, COLOR_BLACK);
    } else {
        // Fallback to standard colors if terminal doesn't support custom colors
        init_pair(GREY_PAIR, COLOR_WHITE, COLOR_BLACK);      // Grey -> White
        init_pair(PINK_PAIR, COLOR_MAGENTA, COLOR_BLACK);    // Pink -> Magenta
        init_pair(MAROON_PAIR, COLOR_RED, COLOR_BLACK);      // Maroon -> Red
        init_pair(LIGHT_BLUE_PAIR, COLOR_CYAN, COLOR_BLACK); // Light Blue -> Cyan
        init_pair(LIGHT_PAIR, COLOR_WHITE, COLOR_BLACK);     // Light -> White
        init_pair(LIGHT_GREEN_PAIR, COLOR_GREEN, COLOR_BLACK); // Light Green -> Green
        init_pair(GOLD_PAIR, COLOR_YELLOW, COLOR_BLACK);     // Gold -> Yellow
        init_pair(BROWN_PAIR, COLOR_YELLOW, COLOR_BLACK);    // Brown -> Yellow
    }
}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL,""); // en_US.UTF-8
    if (!initscr())cerr<<"Could not run\n";
    start_color();
    if (!has_colors())
    {
        printw("This is a monochrome terminal!");
        getch();
    }
    if (!can_change_color())
    {
        printw("This terminal cannot change color!");
        getch();
    }
    init_colors();

    noecho();
    keypad(stdscr,TRUE);
    raw();
    halfdelay(10*THIRD);
    string filepath(".");
    // ----- reserved -----

    unsigned int
        height,
        width,
        min_height = 15,
        min_width = 80;
    getmaxyx(stdscr,height,width);

    // ----- windows -----
    WINDOW* head; // height, width, origin_Y origin_X
    WINDOW* dir;
    WINDOW* view;
    WINDOW* lines;
    WINDOW* editor;
    WINDOW* sweet_patch;
    WINDOW* foot;
    refresh_main:
    if (width<min_width)
    {
        cerr << "Your terminal width is too cramped!\n";
        return 1;
    }
    int dir_width = width * THIRD;
    if (height>min_height)
    {
        head = newwin(4,width,0,0);
        if (current_window_mode == window_modes::RWX_MODE)
        {
            lines = newwin(height-8,6,4,0);
            editor = newwin(height-8,width-6,4,6);
        }
        else
        {
            dir = newwin(height-8,dir_width,4,0);
            view = newwin(height-8,width - dir_width,4,dir_width);
        }
        sweet_patch = newwin(2,width,height-4,0);
        foot = newwin(2,width,height-2,0);
    }
    else
    {
        if (current_window_mode == window_modes::RWX_MODE)
        {
            lines = newwin(height-1,6,0,0);
            editor = newwin(height-1,width-6,0,6);
        }
        else
        {
            dir = newwin(height-1,dir_width,0,0);
            view = newwin(height-1,width - dir_width,0,dir_width);
        }
        sweet_patch = newwin(1,width,height-1,0);
    }
    refresh();

    // ----- draw horizontal lines -----
    if (head) mvwhline(head,3,0,0,width);
    // -------------------------------------------------
    if (dir) box(dir,0,0);
    if (view) box(view,0,0);
    if (lines) box(lines,0,0);
    if (editor) box(editor,0,0);
    // if (sweet_patch) box(sweet_patch,0,0);
    // -------------------------------------------------
    if (foot) mvwhline(sweet_patch,0,0,0,width);
    // ----- draw horizontal lines -----

    // ----- refresh -----
    if (head) wrefresh(head);
    if (dir) wrefresh(dir);
    if (view) wrefresh(view);
    if (lines) wrefresh(lines);
    if (editor) wrefresh(editor);
    if (sweet_patch) wrefresh(sweet_patch);
    if (foot) wrefresh(foot);
    // ----- refresh -----

    // ----- reserved -----
    browse_in_current_directory(dir,view,sweet_patch,filepath);
    refresh();
    getch();

    // ----- reserved -----
    if (head)delwin(head);
    if (dir)delwin(dir);
    if (view)delwin(view);
    if (lines)delwin(lines);
    if (editor)delwin(editor);
    if (sweet_patch)delwin(sweet_patch);
    if (foot)delwin(foot);
    endwin();
    return 0;
    // ----- reserved -----
}




/*
// special chars
/// continuation
// ⤷ ⮩ ⮱
/// current line
// ⮚ 🠊 🠲 🠶 🡆 🡪 🢒 🢥 🢚 ➙
*/