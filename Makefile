Hexagon: *.cpp *.hpp
	g++ -g -O2 -o Hexagon *.cpp -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lGLEW -ljpeg -lXrandr -lfreetype

clean:
	rm Hexagon
