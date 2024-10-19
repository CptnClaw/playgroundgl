CC=clang++
CSTD=-std=c++17
WARN=-Wall -Wextra -Wpedantic -Werror
SOURCE=src/*.cpp src/*/*.cpp
INCLUDE=-Iinclude
LINKGL=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
OUTPUT=playgroundgl
DEBUG=-g3 -O0
RELEASE=-g -O3

COMMON=$(CC) $(CSTD) $(WARN) $(SOURCE) $(INCLUDE) $(LINKGL)

build: 
	$(COMMON) $(RELEASE) -o $(OUTPUT)
debug: 
	$(COMMON) $(DEBUG) -o $(OUTPUT)
clean:
	rm -rf $(OUTPUT)
