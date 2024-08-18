#ifndef ELF_UTIL_HPP
#define ELF_UTIL_HPP

#include <windows.h>
#include <iostream>

#include <fmt/format.h>

#define CHECK(cond, Args, ...)                            \
  {                                                       \
    if (!(cond)) {                                        \
      std::string log = fmt::format(Args, ##__VA_ARGS__); \
      std::cout << __FILE__ << " " << __LINE__ << log;    \
      Sleep(1000);                                        \
      exit(0);                                            \
    }                                                     \
  }

#endif  // ELF_UTIL_HPP