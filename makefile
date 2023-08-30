CC=g++
PKG=sdl2 SDL2_image SDL2_ttf
CFLAGS=-Wall -Wextra -O1 -ggdb `pkg-config --cflags $(PKG)` -ldl
ALGO_FLAGS=-shared -fPIC -Wall -Wextra -O3 -ldl -ggdb
LIBS=`pkg-config --libs $(PKG)`
SRC=src/main.cpp src/Button.cpp src/Grid.cpp src/layout.h

all: search-algo/A-Star search-algo/DFS search-al search-algo/BFS

search-algo/A-Star: src/a-star.cpp src/a-star.h
	g++ -o search-algo/A-Star src/a-star.cpp $(ALGO_FLAGS)

search-algo/DFS: src/dfs.cpp
	g++ -o search-algo/DFS src/dfs.cpp $(ALGO_FLAGS)

search-algo/BFS: src/bfs.cpp
	g++ -o search-algo/BFS src/bfs.cpp $(ALGO_FLAGS)

search-al: $(SRC)
	g++ -o search-al $(SRC) $(CFLAGS) $(LIBS)
