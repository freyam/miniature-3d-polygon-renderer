default:
	g++ main.cpp glad.cpp -o gl -ldl -lglfw && ./gl

compile:
	g++ main.cpp glad.cpp -o gl -ldl -lglfw

run:
	./gl