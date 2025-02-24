# Set the appropriate separator for Windows (backslashes)
SEPARATOR = /

src = $(shell find src -name "*.cpp")
bin = bin/game.html

lib = ./lib/libraylib.a -L./lib
inc = -I./include $(shell find src -type d | sed 's/^/-I/')

pch = ./include/pch.h
pch_o = ./include/pch.h.gch

emflags = -Os \
          -s USE_GLFW=3 \
          -s FETCH=1 \
          -s INITIAL_MEMORY=64MB \
          -s MAXIMUM_MEMORY=256MB \
          -s ALLOW_MEMORY_GROWTH=1 \
	  -s ASSERTIONS=1 \
	  -s NO_DISABLE_EXCEPTION_CATCHING \
          --preload-file assets \
          -pthread

cppflags = -std=gnu++17

$(bin): $(src)
ifeq ($(OS),Windows_NT)
	# Windows command for making the bin directory
	IF NOT EXIST bin mkdir bin
	bear -- emcc -o $(bin) $(src) $(cppflags) $(lib) $(inc) $(emflags) --shell-file ./src/shell.html -DPLATFORM_WEB -include $(pch)
else
	# Unix/MacOS command for making the bin directory
	mkdir -p bin
	bear -- emcc -o $(bin) $(src) $(cppflags) $(lib) $(inc) $(emflags) --shell-file ./src/shell.html -DPLATFORM_WEB -include $(pch)
endif

pch: $(pch)
ifeq ($(OS),Windows_NT)
	# Windows command for emcc on header files
	bear -- emcc $(emflags) -x c++-header $(cppflags) $(pch) -o $(pch_o)
else
	# Unix/MacOS command for emcc on header files
	bear -- emcc $(emflags) -x c++-header $(cppflags) $(pch) -o $(pch_o)
endif

clean:
ifeq ($(OS),Windows_NT)
	# Windows command for cleaning
	del /Q bin\* $(pch_o)
	rmdir /S /Q bin
else
	# Unix/MacOS command for cleaning
	rm -rf bin
	rm -f $(pch_o)
endif

run:
	emrun $(bin)

all: clean pch $(bin)
