#ifndef __glwindow_h__
#define __glwindow_h__

#include <windows.h>
#include <GL/gl.h>

typedef int(*INITFUNC)(void);
typedef int(*RENDERFUNC)(void);
typedef int(*RESHAPEFUNC)(unsigned int, unsigned int);
typedef int(*IDLEFUNC)(void);
typedef int(*DISPOSEFUNC)(void);
typedef int(*KEYBHANDFUNC)(WPARAM wParam, char keyevent);
typedef int(*CLOSEWINFUNC)(void);

typedef struct _glWindow {
    LPCSTR                      windowClass;
    LPCSTR                      windowTitle;
    LPCSTR                      lpIconName;
    unsigned int                startx;
    unsigned int                starty;
    unsigned int                size_w;
    unsigned int                size_h;
    unsigned int                old_size_w;
    unsigned int                old_size_h;
    INITFUNC                    init_func;
    RENDERFUNC                  render_func;
    RESHAPEFUNC                 reshape_func;
    IDLEFUNC                    idle_func;
    DISPOSEFUNC                 dispose_func;
    KEYBHANDFUNC                keybhand_func;
    CLOSEWINFUNC                closewin_func;
}(GLWINDOW);

GLWINDOW *EGL_CreateGLWindow(GLWINDOW *);
HWND EGL_GetWindowHWND(void);
HINSTANCE EGL_GetWindowHINST(void);
HGLRC EGL_GetGLContext(void);
BOOL EGL_GetFullscreenState(void);
void EGL_ShowGLWindow(HINSTANCE, HICON, LPCSTR);
void EGL_SetInitFunction(INITFUNC);
void EGL_SetRenderFunction(RENDERFUNC);
void EGL_SetReshapeFunction(RESHAPEFUNC);
void EGL_SetIdleFunction(IDLEFUNC);
void EGL_SetDisposeFunction(DISPOSEFUNC);
void EGL_SetKeyHandlerFunction(KEYBHANDFUNC);
void EGL_SetWindowCloseFunction(CLOSEWINFUNC);
void EGL_SetWindowTitleText(LPCSTR);
void EGL_SetWindowSize(unsigned int, unsigned int);
void EGL_SetWindowPosition(unsigned int, unsigned int);
void EGL_SetWindowIcon(HICON, HICON, LPCSTR);
void EGL_callInitFunc(void);
void EGL_ToggleFullScreen(void);
void EGL_CloseWindow();

#endif