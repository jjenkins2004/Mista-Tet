# Source files
CFILES = src/*.cpp

# macOS-specific settings (using clang++)
COMPILER_MAC = clang++
SOURCE_LIBS_MAC = -Ilib/
OSX_OPT = -std=c++17 -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib_mac.a
OSX_OUT = -o "bin/MistaTet_Mac"

# Windows-specific settings (using clang++)
COMPILER_WIN = g++
SOURCE_LIBS_WIN = -I lib/
WIN_OPT = -std=c++17 -L lib/ -lraylib_win -lopengl32 -lgdi32 -lwinmm
WIN_OUT = -o "bin/MistaTet_Win.exe"
WIN_STATIC = -static

# macOS build target
mac: 
	$(COMPILER_MAC) $(CFILES) $(SOURCE_LIBS_MAC) $(OSX_OUT) $(OSX_OPT)

# Windows build target
win:
	$(COMPILER_WIN) $(CFILES) $(SOURCE_LIBS_WIN) $(WIN_OUT) $(WIN_OPT) $(WIN_STATIC)