# libenigl

This is a FreeGLUT replacement. I wrote it because I wanted to have a library that handled WINAPI windows for OpenGL, but without the limitations of GLUT and to also be able to use it as a static library, in case I want to embed it into an executable and not depend on any non-native libraries.

It should be able to handle the most basic stuff, and whatever it lacks can be added easily from your program. Just get whatever handles you need and that's it.

Right now it only compiles on MinGW using MSYS, but porting it to VC++ shouldn't be complicated at all.

You can compile it with make running

> make lib

from the src folder.

If you want to install it, you can also use:

> LIBPATH=/path/to/lib INCLUDEPATH=/path/to/GL make install-lib-dev

It will copy the static library to /path/to/lib, and the header to /path/to/GL.

To use the library, import enigl.h from /path/to/GL and then compile with -lenigl .

I didn't write any documentation yet, but you can figure everything out easily from the source (it's not like there is too much going on). But if you need an example, then see [GLCube](https://github.com/aaroncdc/GLCube).