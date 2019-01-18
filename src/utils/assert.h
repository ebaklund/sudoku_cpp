
#ifndef UTILS_ASSERT_H
#define UTILS_ASSERT_H

#include <exception>
#include <string>

#ifdef NDEBUG
#  define ASSERT(expr) void(0);
#else
class AssertException : public std::exception {
  private:
    std::string str;
    static std::string mkWhat(char const* str);

  public:
    AssertException(char const* str)
    : str(mkWhat(str)) {
    }

    virtual char const* what() const noexcept {
      return str.c_str();
    }
};
#  define ASSERT_XSTR(a) ASSERT_STR(a)
#  define ASSERT_STR(a) #a
#  define ASSERT(expr) \
     ((expr) ? void(0) : throw AssertException(ASSERT_XSTR(expr)))
#endif

#endif // UTILS_ASSERT_H
