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

    WINDOW* win_ = newwin(20,35,10,10);
    refresh();
    tml_render render(win_,"name: %s, age: %i.","Joe",19);
    render.render();

    getch();
    endwin();
    return 0;
}
                 

    
    /*
    WINDOW* win = newwin(20,35,10,10);
    init_colors();
    refresh();
    glimpse_inside(win,"/");
    wrefresh(win);
    getch();
    delwin(win);    */