/**
 * file:    main.c
 * author:  wallying@foxmail.com
 * date:    2019-12-30
 **/


#include <windows.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmd, int iCmdShow)
{
    static TCHAR szClassName[] = TEXT("AppWin");
    static TCHAR szWndTitle[] = TEXT("App Demo");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndcls;

    wndcls.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
    wndcls.lpfnWndProc = WndProc;
    wndcls.cbClsExtra = 0;
    wndcls.cbWndExtra = 0;
    wndcls.hInstance = hInst;
    wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndcls.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wndcls.lpszMenuName = NULL;
    wndcls.lpszClassName = szClassName;

    if (!RegisterClass(&wndcls)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"),
                   szClassName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szClassName, szWndTitle,
                        WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
                        330, 150,
                        750, 440,
                        HWND_DESKTOP, NULL, hInst, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int cxChar, cyChar;
    static int cxClient, cyClient;
    HDC hdc;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    RECT rect;


    switch (msg) {
    case WM_CREATE:
        hdc = GetDC(hwnd);

        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cyChar = tm.tmHeight + tm.tmExternalLeading;

        ReleaseDC(hwnd, hdc);
        break;

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        GetClientRect(hwnd, &rect);
        DrawText(hdc, TEXT("Hello World!"), -1, &rect,
                 DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        EndPaint(hwnd, &ps);
        break;

    case WM_VSCROLL:
        break;

    case WM_HSCROLL:
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}


