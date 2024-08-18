COMMON_FLAGS = -g

ELF_CC = gcc
ELF_XX = g++
ELF_FLAGS = -O3
ELF_SRCS = example/elf_example.c
ELF_TARGET = elf.o

elf:
	$(ELF_CC) -c $(ELF_SRCS) $(ELF_FLAGS) $(COMMON_FLAGS) -o $(ELF_TARGET)
	readelf -a elf.o > elf.elf
	objdump -x -s -d elf.o > elf.obj

CC = x86_64-w64-mingw32-gcc-posix
XX = x86_64-w64-mingw32-g++-posix
CFLAGS = 
CXXFLAGS = -D_WIN32_WINNT=0x0A00 -std=c++17 -O3 -fno-stack-protector -fno-asynchronous-unwind-tables -fno-exceptions -fno-stack-check -Wl,--stack,0x100000
LIBS = -lfmt
LIB_PATH =  -Lthird_party/fmt/lib
HEAD_PATH = -Ithird_party/fmt/include
SRCS = main.cpp loader.cpp
TARGET = main

all:
	$(ELF_CC) -c $(ELF_SRCS) $(ELF_FLAGS) $(COMMON_FLAGS) -o $(ELF_TARGET)
	readelf -a elf.o > elf.elf
	objdump -x -s -d elf.o > elf.obj
	$(XX) $(SRCS) $(CXXFLAGS) $(LIBS) $(LIB_PATH) $(HEAD_PATH) -o $(TARGET)

gen:
	readelf -a elf.o > elf.elf

obj:
	objdump -x -s -d elf.o > elf.obj
