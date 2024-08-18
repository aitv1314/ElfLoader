#include <iostream>

#include "loader.hpp"
int main() {
  ElfLoader loader{"./elf.o"};

  int x;
  std::cin >> x;

  return 0;
}