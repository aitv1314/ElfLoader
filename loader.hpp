#ifndef ELF_LOADER_HPP
#define ELF_LOADER_HPP

#include <stdlib.h>
#include <stdio.h>
#include <string_view>

#include <windows.h>

#include "common.h"
#include "elf.hpp"

class ElfLoader {
 public:
  ElfLoader(std::string_view path) { Init(path); }
  ~ElfLoader();

 public:
  void Load();
  void Show();
  void Run();

 private:
  void Init(std::string_view path);

 private:
  bool ValidateCheck() { return true; }  // 检查magic，判断文件类型
  bool SetupLoadInfo();                  // 填充地址

  void LayoutSection();
  void LayoutSymtab();

 private:
  FILE* file_{nullptr};
  void* data_{nullptr};

 private:
  struct {
    unsigned int sym, str, mod, vers, info, pcpu;
  } index;

  Elf64_Ehdr* hdr_;
  Elf64_Shdr* shdr_;

  char *secstrings_, *strtab_;
};

#endif  // ELF_LOADER_HPP