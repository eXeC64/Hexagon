Hexagon: *.cpp *.hpp
	g++ -g -o Hexagon *.cpp -lSDL2 -lcairo

tags: *.cpp *.hpp
	ctags *.cpp *.hpp

clean:
	rm Hexagon
