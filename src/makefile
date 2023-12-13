all:
	gcc -c main.c -o main.o
	gcc main.o -o main -Wl,-subsystem,windows -lopengl32 -lglu32 -lgdi32

winlib:
	gcc -c window.c -o window.o
	gcc -c main.c -o main.o
	gcc window.o main.o -o main -Wl,-subsystem,windows -lopengl32 -lglu32 -lgdi32

windeb:
	gcc -g -c window.c -o window_deb.o
	gcc -g -c main.c -o main_deb.o
	gcc -g -c debug.c -o debug.o
	gcc debug.o window_deb.o main_deb.o -o main -Wl,-subsystem,windows -lopengl32 -lglu32 -lgdi32

lib:
	gcc -c window.c -o window.o
	ar rcs libenigl.a window.o

install-lib:
	gcc -c window.c -o window.o
	ar rcs libenigl.a window.o
	cp libenigl.a /mingw64/lib/libenigl.a
	cp window.h /mingw64/include/GL/enigl.h