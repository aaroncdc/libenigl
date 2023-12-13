lib:
	gcc -c window.c -o window.o
	ar rcs libenigl.a window.o

install-lib:
	gcc -c window.c -o window.o
	ar rcs libenigl.a window.o
	cp libenigl.a ${LIBPATH}/libenigl.a
	cp window.h ${INCLUDEPATH}/enigl.h
	
install-lib-dev:
	gcc -c window.c -o window.o
	ar rcs libenigl.a window.o
	cp libenigl.a /mingw64/lib/libenigl.a
	cp window.h /mingw64/include/GL/enigl.h

clean:
	rm -r *.o
	rm -r *.a