CC=clang++
CSTD=-std=c++17
WARN=-Wall -Wextra -Wpedantic -Werror
SOURCE=src/*.cpp 
INCLUDE=-Isrc 
OUTPUT=playgroundgl
DEBUG=-g3 -O0
RELEASE=-g -O3

build: 
	$(CC) $(CSTD) $(WARN) $(SOURCE) $(RELEASE) $(INCLUDE) -o $(OUTPUT)
debug: 
	$(CC) $(CSTD) $(WARN) $(SOURCE) $(DEBUG) $(INCLUDE) -o $(OUTPUT)
clean:
	rm -rf $(OUTPUT)
