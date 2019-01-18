
#ifndef BOARD_MODEL_H
#define BOARD_MODEL_H

#include "cell_model.h"
#include "../def.h"
#include "../utils/assert.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

class BoardModel {
private:
  std::vector<CellModel> _cells;

public:
  static Def findAnyInBoard(std::function<Def (uint32_t r, uint32_t c)> callback, uint32_t offset = 0) {
    for (uint32_t i = 0; i < 81; ++i) {
      uint32_t ii = (offset + i) % 81;
      Def def = callback(ii / 9, ii % 9);
      if (def.hasValue())
        return def;
    }
    return Def(0,0,0);
  }

  static uint32_t findAnyInCell(std::function<uint32_t (uint32_t n)> callback, uint32_t offset = 0) {
    for (uint32_t i = 0; i < 9; ++i) {
      uint32_t n = callback((offset + i) % 9 + 1);
      if (n != 0)
        return n;
    }
    return 0;
  }

  static void forEachInBoard(std::function<void (uint32_t r, uint32_t c)> callback) {
    for (uint32_t i = 0; i < 81; ++i) {
      callback(i / 9, i % 9);
    }
  }

  static void forEachInRow(uint32_t r, std::function<void (uint32_t r, uint32_t c)> callback) {
    for (uint32_t i = 0; i < 9; ++i) {
      callback(r, i);
    }
  }

  static void forEachInCol(uint32_t c, std::function<void (uint32_t r, uint32_t c)> callback) {
    for (uint32_t i = 0; i < 9; ++i) {
      callback(i, c);
    }
  }

  static void forEachInQuad(uint32_t r, uint32_t c, std::function<void (uint32_t r, uint32_t c)> callback) {
    uint32_t q0 = (r / 3) * 27 + (c / 3) * 3;
    static const uint32_t dq[] = {
       0,  1,  2,
       9, 10, 11,
      18, 19, 20
    };
    for (uint32_t j = 0; j < 9; ++j) {
      uint32_t i = q0 + dq[j];
      callback(i / 9, i % 9);
    }
  }

  BoardModel ()
  : _cells(81) {
  }

  bool isInrange(uint32_t i) const {
    return 0 <= i && i < 81;
  }

  bool areInrange(uint32_t r, uint32_t c) const {
    return 0 <= r && r < 9 && 0 <= c && c < 9;
  }

  CellModel& getCell(uint32_t r, uint32_t c) {
    ASSERT(areInrange(r, c));
    return _cells[9*r + c];
  }

  CellModel const& getCell(uint32_t r, uint32_t c) const {
    ASSERT(areInrange(r, c));
    return _cells[9*r + c];
  }

  bool isAnyUndecided () const {
    return std::any_of(_cells.begin(), _cells.end(), [] (CellModel const & cell) {
      return cell.isUndecided();
    });
  }
};

#endif // BOARD_MODEL_H
