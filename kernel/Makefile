export TARGET=~/opt/cross/bin/i686-elf
IDIR=include
CC=$(TARGET)-gcc
CXX=$(TARGET)-g++
CCFLAGS=-I$(IDIR) -std=gnu99 -ffreestanding -O2
AR=$(TARGET)-ar

SRC_DIR := src
NASM_DIR=asm
OBJ_DIR := bin/o
SRC_FILES := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/sys/*.c)
NASM_FILES := $(wildcard $(NASM_DIR)/*.s)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES)) $(patsubst $(NASM_DIR)/%.s,$(OBJ_DIR)/%.o,$(NASM_FILES))

all: createDirs $(OBJ_FILES)
	$(CC) -T linker.ld -o bin/kernel.bin -ffreestanding -O2 -nostdlib $(OBJ_FILES) -lgcc

createDirs:
	mkdir -p $(OBJ_DIR)/sys
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $< $(CCFLAGS)

$(OBJ_DIR)/%.o: $(NASM_DIR)/%.s
	nasm -felf32 -o $@ $<

.PHONY: clean

clean:
	rm -rf bin/o/*
	rm bin/kernel.bin
