SRC = main.cpp Game.cpp HexagonModel.cpp Util.cpp

Hexagon: $(SRC)
	g++ -g -O2 -o Hexagon $(SRC) ../Uni/Lib110ct/Lib110ct.o -lSDL -lSDL_ttf -lSDL_gfx

clean:
	rm Hexagon
