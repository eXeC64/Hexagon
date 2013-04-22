Hexagon: *.cpp *.hpp
	g++ -g -o Hexagon *.cpp -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lGLEW -ljpeg -lXrandr -lfreetype -lpthread -lrt

tags: *.cpp *.hpp
	ctags *.cpp *.hpp

clean:
	rm Hexagon
