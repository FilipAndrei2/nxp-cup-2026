CC = ./gcc-11.4-arm32-eabi/bin/arm-none-eabi-g++

CFLAGS = -Wall -Wextra -O3 -std=c++20  
TARGET_NAME = out

INCLUDE_DIR = ./include
LIB_DIR     = ./lib

SOURCES = ./src/*.cpp
LIBS = 
LINKER_SCRIPT = ./linker.ld

compile:
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -L $(LIB_DIR) $(SOURCES) -o $(TARGET_NAME) $(LIBS) -T $(LINKER_SCRIPT)
