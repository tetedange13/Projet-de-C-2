gcc -c -Wall maze.c
gcc -c -Wall geom.c
REM gcc -c -Wall graphics.c -lmingw32 -lSDL2main -lSDL2
gcc -c -Wall graphics.c -I include
gcc -c -Wall main.c -I include
gcc -Wall *o -L lib -I include -lmingw32 -lSDL2 -lSDL2main -lSDL2.dll
REM gcc -Wall *.o -L.\lib -I.\include -lmingw32 -lSDL2main -lSDL2 -o toto.exe
del *.o

