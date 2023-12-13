#include "window.h"
#include <GL/glu.h>

GLWINDOW glWindow;
HWND hwnd = 0x0;
HINSTANCE hInstance = 0x0;
HGLRC hRC = 0x0;
char fullscreenmode = 0x0L;
LONG HWNDStyle = 0x0;
HICON wicon = NULL;

const char defaultClass[] = "glWind_0";
const char defaultTitle[] = "OpenGL Application";

HMODULE EGL_GetCurrentModule()
{
    HMODULE hModule = NULL;
    
    #ifndef _win9x
    GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCTSTR)EGL_GetCurrentModule,
        &hModule);
    #else
    hModule = GetModuleHandle(NULL);
    #endif

  return hModule;
}

void EGL_SetDCPixelFormat(HDC hDC)
{
    int nPixelFormat;

    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_GENERIC_ACCELERATED | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24,
        0,0,0,0,0,0,
        0,
        0,
        0,
        0,0,0,0,
        32,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0
    };

        nPixelFormat = ChoosePixelFormat(hDC, &pfd);

        if (nPixelFormat == 0) {
            MessageBox(NULL, "ChoosePixelFormat() failed", "Error", MB_OK); 
            return;
        } 

        SetPixelFormat(hDC, nPixelFormat, &pfd);
        DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
        return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam,
                        LPARAM lParam)
{
    static HDC hDC;

    switch(msg)
    {
        case WM_CREATE:
            hDC = GetDC(hwnd);
            EGL_SetDCPixelFormat(hDC);
            hRC = wglCreateContext(hDC);
            wglMakeCurrent(hDC,hRC);
            if(glWindow.init_func != NULL)
            {
                glWindow.init_func();
            }
            SetTimer(hwnd,101,1,NULL);
        break;
        case WM_DESTROY:
            if(glWindow.dispose_func != NULL)
            {
                glWindow.dispose_func();
            }
            KillTimer(hwnd,101);
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hRC);
            PostQuitMessage(0);
            return wParam;
        break;
        case WM_SIZE:
            //reshape((GLfloat)LOWORD(lParam), (GLfloat)HIWORD(lParam));
            glWindow.size_w = LOWORD(lParam);
            glWindow.size_h = HIWORD(lParam);
            if(glWindow.reshape_func != NULL)
            {
                glWindow.reshape_func(LOWORD(lParam), HIWORD(lParam));
                //glWindow.reshape_func(1024,768);
            }
        break;
        case WM_PAINT:
            //display();
            if(glWindow.render_func != NULL)
            {
                glWindow.render_func();
                SwapBuffers(hDC);
            }
            ValidateRect(hwnd,NULL);
        break;
        case WM_TIMER:
            //display();
            if(glWindow.idle_func != NULL)
            {
                glWindow.idle_func();
            }
            InvalidateRect(hwnd,NULL,FALSE);
        break;
        case WM_KEYDOWN:
        // Keyboard handler
            if(glWindow.keybhand_func != NULL)
            {
                glWindow.keybhand_func(wParam, 0);
            }
        break;
        case WM_KEYUP:
        // Keyboard handler
            if(glWindow.keybhand_func != NULL)
            {
                glWindow.keybhand_func(wParam, 1);
            }
        break;
        case WM_CLOSE:
            if(glWindow.closewin_func != NULL)
            {
                glWindow.closewin_func();
            }
            KillTimer(hwnd,101);
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hRC);
            PostQuitMessage(0);
            DestroyWindow(hwnd);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

GLWINDOW *EGL_CreateGLWindow(GLWINDOW *glWin_)
{
    if(glWin_ != NULL)
    {
        CopyMemory(&glWindow, glWin_, sizeof(GLWINDOW));
        //glw = glWin_;
    }else{
        glWindow.windowClass = defaultClass;
        glWindow.windowTitle = defaultTitle;
        glWindow.lpIconName = IDI_APPLICATION;
        glWindow.startx = 0;
        glWindow.starty = 0;
        glWindow.size_w = 640;
        glWindow.size_h = 480;
        glWindow.old_size_w = glWindow.size_w;
        glWindow.old_size_h = glWindow.size_h;
        glWindow.init_func = NULL;
        glWindow.render_func = NULL;
        glWindow.reshape_func = NULL;
        glWindow.idle_func = NULL;
        glWindow.dispose_func = NULL;
    }

    return &glWindow;
}

void EGL_ShowGLWindow(HINSTANCE hInst_, HICON aIcon, LPCSTR iconName)
{
    MSG msg;
    WNDCLASSEX wc;
    
    hInstance = hInst_;
    //GLWINDOW *glw = &glWindow;

    if(hInstance == NULL)
    {
        hInstance = EGL_GetCurrentModule();
        if(hInstance == NULL)
        {
            hInstance = GetModuleHandle(NULL);
            if(hInstance == NULL)
            {
                MessageBox(NULL, "HINSTANCE not found", "Error", MB_ICONERROR | MB_OK);
                return;
            }
        }
    }

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.lpfnWndProc   = (WNDPROC) WndProc;
    wc.hInstance     = hInstance;
    wc.hIcon         = aIcon ? aIcon : wicon;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = glWindow.windowClass;
    wc.hIconSm       = LoadIcon(NULL, iconName ? iconName : glWindow.lpIconName);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Could not register window.", "Error", MB_ICONERROR | MB_OK);
        return;
    }

    hwnd = CreateWindow(
        glWindow.windowClass,
        glWindow.windowTitle,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        glWindow.startx, glWindow.starty, glWindow.size_w, glWindow.size_h,
        NULL, NULL, hInstance, NULL);

    if(!hwnd)
    {
        MessageBox(NULL, "Could not create window.", "Error", MB_ICONERROR | MB_OK);
        return;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    HWNDStyle = GetWindowLong(hwnd, GWL_STYLE);

    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return;
}

void EGL_ToggleFullScreen()
{
    LONG fsStyle = HWNDStyle;
    fullscreenmode = fullscreenmode ? 0 : 1;

    if(fullscreenmode)
    {
        glWindow.old_size_w = glWindow.size_w;
        glWindow.old_size_h = glWindow.size_h;
        fsStyle &= ~WS_CAPTION;
        fsStyle &= ~WS_THICKFRAME;
    }

    SetWindowLong(hwnd, GWL_STYLE, fsStyle);
    ShowWindow(hwnd, fullscreenmode ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);

    if(!fullscreenmode)
    {
        SetWindowPos(hwnd,NULL,0,0,glWindow.old_size_w,glWindow.old_size_h,0);
    }
    return;
}

HWND EGL_GetWindowHWND()
{
    return hwnd;
}

HINSTANCE EGL_GetWindowHINST()
{
    return hInstance;
}

HGLRC EGL_GetGLContext()
{
    return hRC;
}

BOOL EGL_GetFullscreenState()
{
    return fullscreenmode ? TRUE : FALSE;
}

void EGL_SetInitFunction(INITFUNC init_func)
{
    glWindow.init_func = init_func;
    return;
}

void EGL_SetRenderFunction(RENDERFUNC render_func)
{
    glWindow.render_func = render_func;
    return;
}

void EGL_SetReshapeFunction(RESHAPEFUNC reshape_func)
{
    glWindow.reshape_func = reshape_func;
    return;
}

void EGL_SetIdleFunction(IDLEFUNC idle_func)
{
    glWindow.idle_func = idle_func;
    return;
}

void EGL_SetDisposeFunction(DISPOSEFUNC dispose_func)
{
    glWindow.dispose_func = dispose_func;
    return;
}

void EGL_SetKeyHandlerFunction(KEYBHANDFUNC keybhand_func)
{
    glWindow.keybhand_func = keybhand_func;
    return;
}

void EGL_SetWindowCloseFunction(CLOSEWINFUNC closewin_func)
{
    glWindow.closewin_func = closewin_func;
    return;
}

void EGL_SetWindowTitleText(LPCSTR wTitle)
{
    glWindow.windowTitle = wTitle;
    return;
}

void EGL_SetWindowSize(unsigned int width, unsigned int height)
{
    SetWindowPos(hwnd,NULL,0,0,width,height,SWP_NOREPOSITION);
    return;
}

void EGL_SetWindowPosition(unsigned int x, unsigned int y)
{
    SetWindowPos(hwnd,NULL,x,y,0,0,SWP_NOSIZE);
    return;
}

void EGL_SetWindowIcon(HICON icon_big, HICON icon_small, LPCSTR iconName)
{
    wicon = icon_small;
    if(iconName)
    {
        glWindow.lpIconName = iconName;
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)icon_big);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)icon_small);
    }
    return;
}

void EGL_callInitFunc()
{
    glWindow.init_func();
    return;
}

void EGL_CloseWindow()
{
    SendMessage(EGL_GetWindowHWND(), WM_CLOSE, 0x0L, 0x0L);
    return;
}