#!/bin/bash

gcc -c -Wall maze.c
gcc -c -Wall geom.c
gcc -c -Wall graphics.c 
gcc -Wall *.o `sdl2-config --cflags --libs` -lm

