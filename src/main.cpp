
#include "utils/ncurses_raii.h"
#include "board/board_model.h"
#include "board/board_view.h"
#include "solver/solver.h"

#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <iostream>


int main () {
  //srand (time(NULL));
  srand (2);
  NCursesRaii ncurses_raii;

  BoardModel board;
  BoardView view;
  Solver solver;
  uint32_t defCount = 0;

  Def def = solver.findAnyDefinable(board);
  while (def.hasValue() && defCount < 23) {
    ++defCount;
    solver.define(board, def);
    def = solver.findAnyDefinable(board);
  }

  //view.showDefined(board);
  view.showDecided(board);

	 refresh();
	 getch();

	return 0;
}
