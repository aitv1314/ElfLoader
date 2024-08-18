#ifndef ELF_LOADER_HPP
#define ELF_LOADER_HPP

#include <stdlib.h>
#include <stdio.h>
#include <string_view>

#include "common.h"
#include "libelf/libelf.h"
#include "libelf/gelf.h"

class ElfLoader {
 public:
  ElfLoader(std::string_view path) { Init(path); }
  ~ElfLoader() { free(data_); }

 private:
  void Init(std::string_view path);
  void Load();
  void Show();

  bool ValidateCheck();  // 检查magic，判断文件类型
  bool SetupLoadInfo();  // 填充地址

  void LayoutSection();
  void LayoutSymtab();

 private:
  FILE* file_{nullptr};
  void* data_{nullptr};

 private:
  struct {
    unsigned int sym, str, mod, vers, info, pcpu;
  } index;

  Elf64_Ehdr* hdr;
  Elf64_Shdr* shdr;
};

#endif  // ELF_LOADER_HPP