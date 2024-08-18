#include <iostream>

#include "loader.hpp"
int main() {
  ElfLoader loader{"./elf.o"};
  loader.Load();

  loader.Show();
  loader.Run();

  int x;
  std::cin >> x;

  return 0;
}