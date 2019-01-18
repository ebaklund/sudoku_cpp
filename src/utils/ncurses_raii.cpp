
#include "ncurses_raii.h"

#include <ncurses.h>
#include <stdint.h>

uint32_t g_instance_count = 0;

NCursesRaii::NCursesRaii () {
  if (++g_instance_count == 1) {
	  initscr();
    curs_set(0);
    start_color();
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  }
}

NCursesRaii::~NCursesRaii () {
  if (--g_instance_count == 0) {
	  endwin();
  }
}

