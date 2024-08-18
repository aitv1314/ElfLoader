#include "loader.hpp"

#include <sys/stat.h>

#include "util.hpp"

ElfLoader::~ElfLoader() {
  VirtualFree(data_, 0, MEM_RELEASE);
  // free(data_);
}

void ElfLoader::Init(std::string_view path) {
  std::cout << "func = " << __func__ << std::endl;
  Sleep(1000);

  file_ = fopen(path.data(), "rb");
  CHECK(file_ != nullptr, "");

  struct stat st;
  memset(&st, 0, sizeof(st));
  auto res = fstat(fileno(file_), &st);
  CHECK(res == 0, "");
  fseek(file_, 0, SEEK_SET);

  data_ = VirtualAlloc(0, sizeof(st.st_size), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  //  data_ = malloc(st.st_size);
  CHECK(data_ != nullptr, "");

  res = fread(data_, 1, st.st_size, file_);
  CHECK(res == st.st_size, "res : st.st_size = {}:{}", res, st.st_size);

  fclose(file_);
  file_ = nullptr;
}

void ElfLoader::Load() {
  auto res = ValidateCheck();
  CHECK(res, "");

  res = SetupLoadInfo();
  CHECK(res, "");
}

void ElfLoader::Show() {
  std::cout << std::hex;
  std::cout << "e_ident = \t\t0b" << hdr_->e_ident << std::endl;
  std::cout << "e_type = \t\t0x" << hdr_->e_type << std::endl;
  std::cout << "e_machine = \t\t0x" << hdr_->e_machine << std::endl;
  std::cout << "e_version = \t\t0x" << hdr_->e_version << std::endl;
  std::cout << "e_entry = \t\t0x" << hdr_->e_entry << std::endl;
  std::cout << "e_phoff = \t\t0x" << hdr_->e_phoff << std::endl;
  std::cout << "e_flags = \t\t0x" << hdr_->e_flags << std::endl;
  std::cout << "e_ehsize = \t\t0x" << hdr_->e_ehsize << std::endl;
  std::cout << "e_phentsize = \t\t0x" << hdr_->e_phentsize << std::endl;
  std::cout << "e_phnum = \t\t0x" << hdr_->e_phnum << std::endl;
  std::cout << "e_shentsize = \t\t0x" << hdr_->e_shentsize << std::endl;
  std::cout << "e_shnum = \t\t0x" << hdr_->e_shnum << std::endl;
  std::cout << "e_shstrndx = \t\t0x" << hdr_->e_shstrndx << std::endl << std::endl;
  Sleep(100);
}

bool ElfLoader::SetupLoadInfo() {
  hdr_ = (Elf64_Ehdr*)data_;
  shdr_ = (Elf64_Shdr*)((char*)data_ + hdr_->e_shoff);

  auto res = memcmp(hdr_->e_ident, "ELF", 4);  // !
  CHECK(res > 0, "");

  int i = 0;
  for (i = 1; i < hdr_->e_shnum; i++) {
    if (shdr_[i].sh_type == SHT_SYMTAB) {
      index.sym = i;
      index.str = shdr_[i].sh_link;
      strtab_ = (char*)hdr_ + shdr_[index.str].sh_offset;
      break;
    }
  }

  Elf64_Shdr* strhdr = &shdr_[hdr_->e_shstrndx];
  secstrings_ = (char*)hdr_ + strhdr->sh_offset;

  shdr_[0].sh_addr = 0;
  for (i = 1; i < hdr_->e_shnum; i++) {
    Elf64_Shdr* shdr = &shdr_[i];
    shdr->sh_addr = (size_t)hdr_ + shdr->sh_offset;
  }

  return true;
}

// using FuncType = int(int, int);
typedef int (*FuncType)(int, int);
int call_with(int x, int y, FuncType func) {
  FuncType f = func;
  int result = 0;

  // 使用内联汇编来调用函数指针
  asm volatile (
    "mov %%rcx, %%rdi \n" 
    "mov %%rdx, %%rsi \n"
    "mov %1, %%r8 \n"
    "call *%%r8 \n"
    "mov %%eax, %0\n"
    "ret"
    : "=r"(result)
    : "r"(f)
    : "rdi", "rsi", "rax", "r8", "memory");

  return result;
}

void ElfLoader::Run() {
  Elf64_Sym* sym = nullptr;
  Elf64_Shdr* shdr = nullptr;

  for (int i = 1; i < hdr_->e_shnum; i++) {
    Elf64_Shdr* shdr1 = &shdr_[i];

    std::cout << "shdr->sh_name = \t\t0x" << secstrings_ + shdr1->sh_name << std::endl;
    std::cout << "shdr->sh_type = \t\t0x" << shdr1->sh_type << std::endl;
    std::cout << "shdr->sh_flags = \t\t0x" << shdr1->sh_flags << std::endl;
    std::cout << "shdr->sh_addr = \t\t0x" << shdr1->sh_addr << std::endl;
    std::cout << "shdr->sh_offset = \t\t0x" << shdr1->sh_offset << std::endl;
    std::cout << "shdr->sh_size = \t\t0x" << shdr1->sh_size << std::endl;
    std::cout << "shdr->sh_link = \t\t0x" << shdr1->sh_link << std::endl;
    std::cout << "shdr->sh_info = \t\t0x" << shdr1->sh_info << std::endl;
    std::cout << "shdr->sh_addralign = \t\t0x" << shdr1->sh_addralign << std::endl;
    std::cout << "shdr->sh_entsize = \t\t0x" << shdr1->sh_entsize << std::endl << std::endl;

    if (shdr1->sh_type == SHT_SYMTAB) {
      sym = (Elf64_Sym*)shdr1->sh_addr;
      shdr = shdr1;
    }
  }

  std::cout << "sym:shdr = " << (uint64_t)sym << " " << (uint64_t)shdr << std::endl;

  CHECK(sym != nullptr, "");
  CHECK(shdr != nullptr, "");

  std::cout << "*******************************************************" << std::endl << std::endl;
  std::cout << "shdr->sh_name = \t\t0x" << secstrings_ + shdr->sh_name << std::endl;
  std::cout << "shdr->sh_type = \t\t0x" << shdr->sh_type << std::endl;
  std::cout << "shdr->sh_flags = \t\t0x" << shdr->sh_flags << std::endl;
  std::cout << "shdr->sh_addr = \t\t0x" << shdr->sh_addr << std::endl;
  std::cout << "shdr->sh_offset = \t\t0x" << shdr->sh_offset << std::endl;
  std::cout << "shdr->sh_size = \t\t0x" << shdr->sh_size << std::endl;
  std::cout << "shdr->sh_link = \t\t0x" << shdr->sh_link << std::endl;
  std::cout << "shdr->sh_info = \t\t0x" << shdr->sh_info << std::endl;
  std::cout << "shdr->sh_addralign = \t\t0x" << shdr->sh_addralign << std::endl;
  std::cout << "shdr->sh_entsize = \t\t0x" << shdr->sh_entsize << std::endl << std::endl;

  std::cout << "*******************************************************" << std::endl;
  auto size = shdr->sh_size / shdr->sh_entsize;
  LOG("size:shdr->sh_size:shdr->sh_entsize:sizeof = {}:{}:{}:{}", size, shdr->sh_size, shdr->sh_entsize, sizeof(*sym));

  std::cout << "*******************************************************" << std::endl;
  Elf64_Sym* target_symbol = nullptr;
  for (int j = 0; j < size; ++j) {
    std::cout << j << " sym->st_name = \t\t0b" << strtab_ + sym[j].st_name << std::endl;
    std::cout << j << " sym->st_info = \t\t0x" << sym[j].st_info << std::endl;
    std::cout << j << " sym->st_other = \t\t0x" << sym[j].st_other << std::endl;
    std::cout << j << " sym->st_shndx = \t\t0x" << sym[j].st_shndx << std::endl;
    std::cout << j << " sym->st_value = \t\t0x" << sym[j].st_value << std::endl;
    std::cout << j << " sym->st_size = \t\t0x" << sym[j].st_size << std::endl;
    std::cout << "*******************************************************" << std::endl;
    if (strcmp(strtab_ + sym[j].st_name, "Enter") == 0) {
      target_symbol = &sym[j];
    }
  }

  if (target_symbol) {
    Elf64_Shdr* text_sec = &shdr_[target_symbol->st_shndx];
    void* enterance = (char*)text_sec->sh_addr + target_symbol->st_value;
    // *(char*)enterance = 0x90;
    // *((char*)enterance + 1) = 0x90;
    // *((char*)enterance + 2) = 0x90;
    // *((char*)enterance + 3) = 0x90;

    FuncType func = (FuncType)(enterance);

    LOG("target_symbol = 0x{:x}, text_sec->sh_addr = 0x{:x}, st_value = {}, func = 0x{:x}", (uint64_t)target_symbol,
        (uint64_t)text_sec->sh_addr, target_symbol->st_value, (uint64_t)func);

    uint32_t instr = *(uint32_t*)func;
    LOG("first instr = {:x}", instr);

    int result = call_with(1, 3, func);

    std::cout << std::dec << "end,result = " << result << std::endl;  // << " b = " << b <<
  }
}