CC=g++
PKG=sdl2 SDL2_image SDL2_ttf
CFLAGS=-Wall -Wextra -O3 -ggdb `pkg-config --cflags $(PKG)` -ldl
ALGO_FLAGS=-shared -fPIC -Wall -Wextra -O3 -ldl -ggdb
LIBS=`pkg-config --libs $(PKG)`
HEADER=src/*.h
SRC=src/main.cpp src/Button.cpp src/Grid.cpp src/text.cpp

all: search-algo/A-Star search-algo/DFS search-al search-algo/BFS

search-algo/A-Star: src/a-star.cpp src/a-star.h
	$(CC) -o search-algo/A-Star src/a-star.cpp $(ALGO_FLAGS)

search-algo/DFS: src/dfs.cpp
	$(CC) -o search-algo/DFS src/dfs.cpp $(ALGO_FLAGS)

search-algo/BFS: src/bfs.cpp
	$(CC) -o search-algo/BFS src/bfs.cpp $(ALGO_FLAGS)

search-al: $(SRC) $(HEADER)
	$(CC) -o search-al $(SRC) $(CFLAGS) $(LIBS)
