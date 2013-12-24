Hexagon: *.cpp *.hpp
	g++ -g -o Hexagon *.cpp -lsfml-graphics -lsfml-window -lsfml-system -lGLEW -ljpeg -lXrandr -lfreetype -lpthread -lrt

tags: *.cpp *.hpp
	ctags *.cpp *.hpp

clean:
	rm Hexagon
