default:
	g++ src/*.cpp -Wall -Wextra -O1 -o A_start `pkg-config --libs --cflags sdl2 SDL2_image SDL2_ttf` -fsanitize=address,undefined
