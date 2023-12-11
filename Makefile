CFLAGS=-Wall -Wextra -Wformat=2 -Wimplicit-fallthrough -std=c99
LDLIBS=-lSDL2 -lm

lint:
	cppcheck ./src

build: lint
	clang $(CFLAGS)             src/*.c $(LDLIBS) -o ./build/renderer

build-debug:
	clang $(CFLAGS) -Werror -g  src/*.c $(LDLIBS) -o ./build/renderer

build-release:
	clang $(CFLAGS) -O3 src/*.c $(LDLIBS) -o ./build/renderer

run:
	./build/renderer

tools-mac:
	brew install llvm cppcheck
