run: lab2.cpp
	g++ lab2.cpp lab2InitShaders.cpp -lglut -lGLU -lGL -lGLEW -g
	./a.out

clean: 
	rm -f *.out *~ run
