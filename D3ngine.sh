g++ -c engine3d/main.cpp -Iusr/include/SFMl
g++ main.o -o engine3d/main -Lusr/include/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system
cd engine3d
./main
