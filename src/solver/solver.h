
#ifndef SOLVER_H
#define SOLVER_H

#include "../def.h"


class BoardModel;
class Def;

class Solver {
public:
  Solver();
  static Def findAnyDefinable (BoardModel const& board);
  static void define (BoardModel& board, Def const& def);
};

#endif // SOLVER_H
