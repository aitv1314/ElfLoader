#ifndef ELF_UTIL_HPP
#define ELF_UTIL_HPP

#include <windows.h>
#include <iostream>

#include <fmt/format.h>

#define CHECK(cond, Format, ...)                              \
  {                                                           \
    if (!(cond)) {                                            \
      std::string log = fmt::format(Format, ##__VA_ARGS__);   \
      std::cout << __FILE__ << " " << __LINE__ << " " << log; \
      Sleep(1000);                                            \
      exit(0);                                                \
    }                                                         \
  }
#define LOG(Format, ...)                                                 \
  {                                                                      \
    std::string log = fmt::format(Format, ##__VA_ARGS__);                \
    std::cout << __FILE__ << " " << __LINE__ << " " << log << std::endl; \
  }

#endif  // ELF_UTIL_HPP