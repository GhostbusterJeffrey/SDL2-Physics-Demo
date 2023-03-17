all:
	g++ -IPath\to\SDL2\include -LPath\to\SDL2\lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2
	main.exe
