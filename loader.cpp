#include "loader.hpp"

#include <sys/stat.h>

#include "util.hpp"

void ElfLoader::Init(std::string_view path) {
  file_ = fopen(path.data(), "w+");
  CHECK(file_ != nullptr, "");

  struct stat st;
  auto res = stat(path.data(), &st);
  CHECK(res == 0, "");

  data_ = malloc(st.st_size);
  CHECK(data_ != nullptr, "");

  res = fread(data_, 1, st.st_size, file_);
  CHECK(res == st.st_size, "");

  fclose(file_);
  file_ = nullptr;
}

void ElfLoader::Load() {
  auto res = ValidateCheck();
  CHECK(res, "");
}
