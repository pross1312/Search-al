CC=g++
PKG=sdl2 SDL2_image SDL2_ttf
CFLAGS=-Wall -Wextra -O1 `pkg-config --cflags $(PKG)`
LIBS=`pkg-config --libs $(PKG)`
SRC=src/main.cpp src/Button.cpp src/Grid.cpp src/a-star.cpp

A_start: $(SRC)
	g++ -o A_star $(SRC) $(CFLAGS) $(LIBS)
