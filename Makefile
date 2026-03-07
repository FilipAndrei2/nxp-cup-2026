CC = ./gcc-11.4-arm32-eabi/bin/arm-none-eabi-g++

CFLAGS = -Wall -Wextra -O3 -std=c++20 
TARGET_NAME = out

INCLUDE_DIR = ./include
SOURCE_FILES = ./src/*.cpp

compile:
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) $(SOURCE_FILES) -o $(TARGET_NAME)  
