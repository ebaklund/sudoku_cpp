#include "solver.h"
#include "../board/board_model.h"

#include <stdlib.h>
#include <iostream>


void reduceCell (BoardModel& board, uint32_t r, uint32_t c, uint32_t n);

void reduceRow(BoardModel& board, uint32_t r, uint32_t irreducible_c, uint32_t n) {
  BoardModel::forEachInRow(r, [&] (uint32_t r, uint32_t c) {
    if (c != irreducible_c)
      reduceCell(board, r, c, n);
  });
}

void reduceCol (BoardModel& board, uint32_t irreducible_r, uint32_t c, uint32_t n) {
  BoardModel::forEachInCol(c, [&] (uint32_t r, uint32_t c) {
    if (r != irreducible_r)
      reduceCell(board, r, c, n);
  });
}

void reduceQuad (BoardModel& board, uint32_t irreducible_r, uint32_t irreducible_c, uint32_t n) {
  BoardModel::forEachInQuad(irreducible_r, irreducible_c, [&] (uint32_t r, uint32_t c) {
    if (r != irreducible_r || c != irreducible_c)
      reduceCell(board, r, c, n);
  });
}

void reduceAllInflicted (BoardModel& board, uint32_t r, uint32_t c, uint32_t n) {
    reduceRow(board, r, c, n);
    reduceCol(board, r, c, n);
    reduceQuad(board, r, c, n);
}

void reduceCell (BoardModel& board, uint32_t r, uint32_t c, uint32_t n) {
  CellModel& cell = board.getCell(r, c);

  ASSERT(!cell.isDecided(n));
  ASSERT(!cell.isDefined(n));

  if (cell.isPossible(n)) {
    cell.reducePossibilities(n);
    if (cell.isDecided())
      reduceAllInflicted(board, r, c, cell.getDecision());
  }
}


static bool isSingleInRow (BoardModel const& board, uint32_t r, uint32_t candidate_c, uint32_t n) {
  ASSERT(board.getCell(r, candidate_c).isPossible(n));

  bool unique = true;
  BoardModel::forEachInRow(r, [&] (uint32_t r, uint32_t c) {
    if (c != candidate_c) {
      unique &= !board.getCell(r, c).isPossible(n);
    }
  });
  return unique;
}

static bool isSingleInCol (BoardModel const& board, uint32_t candidate_r, uint32_t c, uint32_t n) {
  ASSERT(board.getCell(candidate_r, c).isPossible(n));

  bool unique = true;
  BoardModel::forEachInCol(c, [&] (uint32_t r, uint32_t c) {
    if (r != candidate_r) {
      unique &= !board.getCell(r, c).isPossible(n);
    }
  });
  return unique;
}

static bool isSingleInQuad (BoardModel const& board, uint32_t candidate_r, uint32_t candidate_c, uint32_t n) {
  ASSERT(board.getCell(candidate_r, candidate_c).isPossible(n));

  bool unique = true;
  BoardModel::forEachInQuad(candidate_r, candidate_c, [&] (uint32_t r, uint32_t c) {
    if (r != candidate_r || c != candidate_c) {
      unique &= !board.getCell(r, c).isPossible(n);
    }
  });
  return unique;
}

static bool isSingletonInCell (BoardModel const& board, uint32_t r, uint32_t c, uint32_t n) {
  return
    board.getCell(r, c).isPossible(n) && (
    isSingleInRow(board, r, c, n) ||
    isSingleInCol(board, r, c, n) ||
    isSingleInQuad(board, r, c, n));
}

static uint32_t findAnySingletonInCell (BoardModel const& board, uint32_t r, uint32_t c) {
  if (board.getCell(r, c).isUndecided())
    return BoardModel::findAnyInCell([&] (uint32_t n) {
      return isSingletonInCell(board, r, c, n) ? n : 0;
    });
  else
    return 0;
}

static void decideAllSingletons(BoardModel& board) {
  BoardModel::forEachInBoard([&] (uint32_t r, uint32_t c) {
    uint32_t n = findAnySingletonInCell(board, r, c);
    if (n != 0)
      board.getCell(r, c).setDecision(n);
  });
}

static uint32_t findAnyUndecidedInCell (BoardModel const& board, uint32_t r, uint32_t c) {
  CellModel const& cell = board.getCell(r, c);
  if (cell.isUndecided())
    return BoardModel::findAnyInCell([&] (uint32_t n) {
      return cell.isPossible(n) ? n : 0;
    }, rand() % 9);
  else
    return 0;
}

static Def findAnyUndecidedOnBoard (BoardModel const& board) {
  ASSERT(board.isAnyUndecided());

  return BoardModel::findAnyInBoard([&] (uint32_t r, uint32_t c) {
    uint32_t n = findAnyUndecidedInCell(board, r, c);
    if (n != 0)
      return Def(r, c, n);
  }, rand() % 81);
}

Solver::Solver () {
}

Def Solver::findAnyDefinable (BoardModel const& board) {
  return findAnyUndecidedOnBoard(board);
}

void Solver::define (BoardModel& board, Def const& d) {
  uint32_t r = d.r;
  uint32_t c = d.c;
  uint32_t n = d.n;

  ASSERT(board.getCell(r, c).isPossible(n));
  ASSERT(!board.getCell(r, c).isDecided(n));
  ASSERT(!board.getCell(r, c).isDefined(n));

  board.getCell(r, c).setDefinition(n);
  reduceAllInflicted(board, r, c, n);
  decideAllSingletons(board);
}

