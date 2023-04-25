OBJS = bai.cpp Area.cpp Function.cpp LTexture.cpp heart.cpp draw.cpp show.cpp
OBJ_NAME = bai.exe
COMPILER_FLAGS = -std=c++17
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDE_DIR = -Iinclude
LIB_DIR = -Llib

compile:
	g++ $(COMPILER_FLAGS) $(INCLUDE_DIR) $(LIB_DIR) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)