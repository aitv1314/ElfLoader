COMMON_FLAGS = -g -O3

ELF_CC = gcc
ELF_XX = g++
ELF_FLAGS = 
ELF_SRCS = example/elf_example.c
ELF_TARGET = elf.o

elf:
	$(ELF_CC) -c $(ELF_SRCS) $(ELF_FLAGS) $(COMMON_FLAGS) -o $(ELF_TARGET)

CC = x86_64-w64-mingw32-gcc-posix
XX = x86_64-w64-mingw32-g++-posix
CFLAGS = 
CXXFLAGS = -D_WIN32_WINNT=0x0A00 -std=c++20
LIBS = -lelf -lfmt
LIB_PATH = -Lthird_party/libelf/lib -Lthird_party/fmt/lib
HEAD_PATH = -Ithird_party/libelf/include -Ithird_party/fmt/include
SRCS = main.cpp loader.cpp
TARGET = main
all:
	$(XX) $(SRCS) $(CXXFLAGS) $(LIBS) $(LIB_PATH) $(HEAD_PATH) -o $(TARGET)