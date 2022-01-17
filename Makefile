default:
	g++ main.cpp glad.cpp -o 3D -ldl -lglfw && ./3D $(NUM_VERTICES)

compile:
	g++ main.cpp glad.cpp -o 3D -ldl -lglfw

run:
	./3D $(NUM_VERTICES)