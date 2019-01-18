
#ifndef CELL_MODEL_H
#define CELL_MODEL_H

#include "../utils/assert.h"
#include <stdint.h>

enum CellStateEnum {
  Unchanged,
  Unsolved,
  Solved
};

class CellModel {

private:
  //   bitset : 16
  //     size :  8
  // solution :  8
  uint32_t _state;

  uint32_t getPossibilities () const {
    return _state & 0x03FE;
  }

  void setPossibilities (uint32_t p) {
    ASSERT(p < 2 || 0x03FE > p);
    ASSERT(2 <= p && p <= 0x03FE);
    _state &= 0xFFFF0000;
    _state |= p;
  }

  void setSize (uint32_t s) {
    _state &= 0xFF00FFFF;
    _state |= (s << 16);
  }

  uint32_t getOnePossibility () const {
    ASSERT(getPossibilities() != 0);
    uint32_t p = getPossibilities();
    uint32_t n = 0;
    while(!(p & (1 << ++n)));
    return n;
  }

public:

  CellModel ()
    : _state(0x000903FE) {
  }

  static bool isInrange(uint32_t n) {
    return 1 <= n && n <= 9;
  }

  uint32_t getSize () const {
    return 0xFF & (_state >> 16);
  }

  bool isUndecided () const {
    return getSize() > 1;
  }

  bool isPossible (uint32_t n) const {
    ASSERT(isInrange(n));
    return (getPossibilities() & (1 << n)) != 0;
  }

  bool isDecided () const {
    return getSize() == 1;
  }

  bool isDecided (uint32_t n) const {
    ASSERT(isInrange(n));
    return isPossible(n) && isDecided();
  }

  void setDecision (uint32_t n) {
    ASSERT(isInrange(n));
    ASSERT(isPossible(n));
    _state = 0x00010000 | (1 << n);
  }

  void reducePossibilities (uint32_t n) {
    ASSERT(isInrange(n));
    ASSERT(!isDecided(n));
    ASSERT(!isDefined(n));

    if (isPossible(n)) {
      _state ^= (1 << n);
      setSize(getSize() - 1);
    }
  }

  bool isDefined () const {
    return getDefinition() != 0;
  }

  bool isDefined (uint32_t n) const {
    return getDefinition() == n;
  }

  uint32_t getDecision () const {
    return isDecided() ? getOnePossibility() : 0;
  }

  uint32_t getDefinition () const {
    return _state >> 24;
  }

  void setDefinition (uint32_t n) {
    ASSERT(isInrange(n));
    _state = (n << 24) | 0x00010000 | (1 << n);
  }
};

#endif // CELL_MODEL_H
