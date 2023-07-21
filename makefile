CC=g++
PKG=sdl2 SDL2_image SDL2_ttf
CFLAGS=-Wall -Wextra -O1 -ggdb `pkg-config --cflags $(PKG)` -ldl
LIBS=`pkg-config --libs $(PKG)`
SRC=src/main.cpp src/Button.cpp src/Grid.cpp src/widget-manager.cpp

all: search-algo/A-Star search-algo/DFS A_star

search-algo/A-Star: src/a-star.cpp src/a-star.h
	g++ -shared  -fPIC -Wall -Wextra -o search-algo/A-Star src/a-star.cpp -ldl

search-algo/DFS: src/dfs.cpp
	g++ -shared  -fPIC -Wall -Wextra -o search-algo/DFS src/dfs.cpp -ldl

A_star: $(SRC)
	g++ -o A_star $(SRC) $(CFLAGS) $(LIBS)
