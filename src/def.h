
#ifndef DEF_H
#define DEF_H

#include "utils/assert.h"
#include <stdint.h>


struct Def {
  uint32_t r, c, n;

  Def() = delete;

  Def(Def const& d)
  : Def(d.r, d.c, d.n) {
  }

  Def (uint32_t r, uint32_t c, uint32_t n)
  : r(r), c(c), n(n) {
    ASSERT(0 <= r && r <= 8 && "r must be within row range");
    ASSERT(0 <= c && c <= 8 && "c must be within column range");
    ASSERT(n <= 9 && "n must be within value range");
  }

  bool hasValue() const {
    return n != 0;
  }
};

#endif // DEF_H
