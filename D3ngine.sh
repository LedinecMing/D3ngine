g++ -c main.cpp -I[sfml include path]
g++ main.o -o main -L[sfml lib path] -lsfml-graphics -lsfml-window -lsfml-system
./main
