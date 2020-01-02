/**
 * file:    main.c
 * author:  wallying@foxmail.com
 * date:    2019-12-31
 **/


#include <stdio.h>
#include <windows.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmd, int iCmdShow)
{
    static TCHAR szClassName[] = TEXT("AppWin");
    static TCHAR szWndTitle[] = TEXT("App Demo");

    WNDCLASS wndcls;
    HWND hwnd;
    MSG msg;


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
        MessageBox(NULL, TEXT("ERROR: program requires Windows NT!"),
                   szClassName, MB_ICONERROR);
        return 0;
    }


    hwnd = CreateWindow(szClassName, szWndTitle,
                        WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
                        400, 180,
                        550, 370,
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
    static int cxChar, cxCaps, cyChar;
    static int cxClient, cyClient;
    static int iHrozPos, iVertPos;
    HDC hdc;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    RECT rect;
    TCHAR szBuf[50];
    SCROLLINFO si;


    switch (msg) {
    case WM_CREATE: /* 1 */
        hdc = GetDC(hwnd);

        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
        cyChar = tm.tmHeight + tm.tmExternalLeading;
        printf("Metrics(%d,%d,%d)\n", cxChar, cxCaps, cyChar);

        ReleaseDC(hwnd, hdc);
        break;

    case WM_SIZE: /* 5 */
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        printf("WM_SIZE(%d,%d)\n", cxClient, cyClient);

        si.cbSize = sizeof(si);
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax = 100;
        si.nPage = cxClient / cxChar;
        SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

        si.cbSize = sizeof(si);
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax = 100;
        si.nPage = cyClient / cyChar;
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        break;

    case WM_PAINT: /* 15 */
        hdc = BeginPaint(hwnd, &ps);

        si.cbSize = sizeof(si);
        si.fMask = SIF_POS;
        GetScrollInfo(hwnd, SB_HORZ, &si);
        iHrozPos = si.nPos;
        GetScrollInfo(hwnd, SB_VERT, &si);
        iVertPos = si.nPos;

        GetClientRect(hwnd, &rect);
        TextOut(hdc, rect.right / 2, rect.bottom / 2, szBuf,
                wsprintf(szBuf, TEXT("Hello World!")));

        EndPaint(hwnd, &ps);
        break;

    case WM_HSCROLL: /* 276 */
        break;

    case WM_VSCROLL: /* 277 */
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(hwnd, SB_VERT, &si);

        iVertPos = si.nPos;
        switch (LOWORD(wParam)) {
        case SB_TOP:
            si.nPos = si.nMin;
            break;
        case SB_BOTTOM:
            si.nPos = si.nMax;
            break;
        case SB_LINEUP:
            si.nPos -= 1;
            break;
        case SB_LINEDOWN:
            si.nPos += 1;
            break;
        case SB_PAGEUP:
            si.nPos -= si.nPage;
            break;
        case SB_PAGEDOWN:
            si.nPos += si.nPage;
            break;
        case SB_THUMBPOSITION:
            si.nPos = si.nTrackPos;
            break;
        default:
            break;
        }

        si.fMask = SIF_POS;
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        GetScrollInfo(hwnd, SB_VERT, &si);
        if (si.nPos != iVertPos) {
            ScrollWindow(hwnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL);
            UpdateWindow(hwnd);
        }
        break;

    case WM_DESTROY: /* 2 */
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

