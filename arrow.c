#define WINVER 0x0500  


#include <windows.h>  
#include <stdlib.h>  
#include <string.h>   
  
// Global variables  
  
// The main window class name.  
static char szWindowClass[] = "win32app";  
  
// The string that appears in the application's title bar.  
static char szTitle[] = "Arrow Window";  
  
HINSTANCE hInst;  

HBITMAP loadedbitmap = NULL;
  
// Forward declarations of functions included in this code module:  
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);  
  
int CALLBACK WinMain( HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR lpCmdLine,  int nCmdShow)  
{ 

    WNDCLASSEX wcex;  
  
    wcex.cbSize = sizeof(WNDCLASSEX);  
    wcex.style          = CS_HREDRAW | CS_VREDRAW;  
    wcex.lpfnWndProc    = WndProc;  
    wcex.cbClsExtra     = 0;  
    wcex.cbWndExtra     = 0;  
    wcex.hInstance      = hInstance;  
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));  
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);  
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);  
    wcex.lpszMenuName   = NULL;  
    wcex.lpszClassName  = szWindowClass;  
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));  
  
    if (!RegisterClassEx(&wcex))  
    {  
        MessageBox(NULL,  
            "Call to RegisterClassEx failed!",  
            "Win32 Guided Tour",  
            0);  
  
        return 1;  
    }  
  
    hInst = hInstance; // Store instance handle in our global variable  
  
    // The parameters to CreateWindow explained:  
    // szWindowClass: the name of the application  
    // szTitle: the text that appears in the title bar  
    // WS_OVERLAPPEDWINDOW: the type of window to create  
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)  
    // 500, 100: initial size (width, length)  
    // NULL: the parent of this window  
    // NULL: this application does not have a menu bar  
    // hInstance: the first parameter from WinMain  
    // NULL: not used in this application  
    HWND hWnd = CreateWindow(  
        szWindowClass,  
        szTitle,  
        WS_POPUP | WS_MAXIMIZE,  
        CW_USEDEFAULT, CW_USEDEFAULT,  
        500, 100,  
        NULL,  
        NULL,  
        hInstance,  
        NULL  
    );  
  
    if (!hWnd)  
    {  
        MessageBox(NULL,  
            "Call to CreateWindow failed!",  
            "Win32 Guided Tour",  
            0);  
  
        return 1;  
    }  


    loadedbitmap = (HBITMAP)LoadImage(hInst, "C:\\Users\\user\\Desktop\\arrow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  
    // The parameters to ShowWindow explained:  
    // hWnd: the value returned from CreateWindow  
    // nCmdShow: the fourth parameter from WinMain  
    ShowWindow(hWnd,  
        nCmdShow);  
    UpdateWindow(hWnd);  

SetWindowPos(hWnd, HWND_TOPMOST, 0,  0, GetSystemMetrics(SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ), 0 );
      MSG msg;  
    if(RegisterHotKey(NULL,1, MOD_ALT | 0x4000 ,0x57)) { 

    while (GetMessage(&msg, NULL, 0, 0))  
    {  
        TranslateMessage(&msg);  
        DispatchMessage(&msg);  
    } 
}
  
    return (int) msg.wParam;  
}  
  
//  
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)  
//  
//  PURPOSE:  Processes messages for the main window.  
//  
//  WM_PAINT    - Paint the main window  
//  WM_DESTROY  - post a quit message and return  
//  
//  
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)  
{  
    PAINTSTRUCT ps;  
    HDC hdc;  
    char greeting[] = "Win32 window test";  
  
    switch (message)  
    {  
        case WM_HOTKEY:
            ExitProcess(0);
            break;
    case WM_PAINT:  
        hdc = BeginPaint(hWnd, &ps); 

        BITMAP          bitmap;
        HGDIOBJ         oldBitmap;

        hdc = BeginPaint(hWnd, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, loadedbitmap);

        GetObject(loadedbitmap, sizeof(bitmap), &bitmap);
        BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);


        TextOut(hdc,  
            5, 5,  
            greeting, strlen(greeting));  
        // End application-specific layout section.  
  
        EndPaint(hWnd, &ps);  
        break;  
    case WM_DESTROY:  
        PostQuitMessage(0);  
        break;
    default:  
        return DefWindowProc(hWnd, message, wParam, lParam);  
        break;  
    }  
  
    return 0;  
}  


void networkingThread() {


}