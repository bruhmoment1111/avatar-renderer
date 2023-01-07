build:
	gcc main.c -lwebp libs/libTinyGL.a libs/objloader.h -lm -g -fopenmp -lm -o main && ./main