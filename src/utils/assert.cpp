
#include "assert.h"

#include <libunwind.h>
#include <cxxabi.h>
#include <strstream>

#ifndef NDEBUG

std::string AssertException::mkWhat(char const* str) {
  std::strstream ss;
  ss << str << "\n";

  unw_cursor_t cursor;
  unw_context_t context;

  // Initialize cursor to current frame for local unwinding.
  unw_getcontext(&context);
  unw_init_local(&cursor, &context);

  // Unwind frames one by one, going up the frame stack.
  int i = 0;
  while (unw_step(&cursor) > 0) {
    if (i-- == 0) {
      continue;
    }
    unw_word_t offset, pc;
    unw_get_reg(&cursor, UNW_REG_IP, &pc);
    if (pc == 0) {
      break;
    }

    ss << "\t0x" << std::hex << pc;

    char sym[256];
    char* name = sym;

    if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
      int status;
      if ( (name = abi::__cxa_demangle(sym, NULL, NULL, &status)) == 0 ) {
        name = sym;
      }
      ss << " " << name << "; (0x" << std::hex << offset << ")\n";
      if ( name != sym ) {
        free(name);
      }
    }
    else {
      ss << " -- error: unable to obtain symbol name for this frame\n";
    }
  }

  return ss.str();
}

#endif