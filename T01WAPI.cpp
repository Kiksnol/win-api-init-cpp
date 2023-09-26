/* FILE NAME  : T00FST.cpp
 * PURPOSE  : WinAPI first project
 * PROGRAMMER : ND4
 * LAST UPDATE: 17.07.2021
 */



#include <windows.h>

#define WND_CLASS_NAME L"First sum camp project"

LRESULT CALLBACK WindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

/* Main WINAPI function.
 * ARGUMENTS:
 *   - Window descriptor:
 *       HINSTANCE hInstance;
 *   - Previous window descriptor:
 *       HINSTANCE hPrevInstance;
 *   - Command line:
 *       LPSTR CmdLine;
 *   - Window show flag:
 *       INT ShowCmd;
 *  RETURNS:
 *  (INT) returning code to OS;
 */
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR CmdLine, INT SHowCmd)
{
    WNDCLASS wc;
    HWND hWnd;
    MSG msg;

    wc.style = CS_VREDRAW | CS_HREDRAW;

    wc.cbClsExtra = 0; /*  */
    wc.cbWndExtra = 0; /*  */
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* Background color */
    wc.hCursor = LoadCursor(NULL, IDC_CROSS); /* Cursor Load */
    wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION); /* Icon Load */
    wc.hInstance = hInstance; /* Window register class description */
    wc.lpfnWndProc = WindowFunc; /**/
    wc.lpszClassName = WND_CLASS_NAME; /* Window class name */
    wc.lpszMenuName = NULL;

    /* Class registration */
    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, L"Error register window class", ERROR, MB_OK);
        return 0;
    }

    hWnd = CreateWindow(WND_CLASS_NAME,
        L"ND4 1st proj", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL,
        hInstance, NULL);

    /* Show and redraw window */
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    /* Message get until WM_QUIT */
    while (GetMessage(&msg, NULL, 0, 0))
    {
        /*  */
        TranslateMessage(&msg);

        /*  */
        DispatchMessage(&msg);
    }

    return msg.wParam;
} /* End of 'WindowFunc' function */

/* LRESULT CALLBACK Window function.
 * ARGUMENTS:
 *   - Window descriptor:
 *     HWND hWnd;
 *   - Message number:
 *     UINT Msg;
 *   - Word Parameter:
 *     WPARAM wParam;
 *   - Long Parameter:
 *     LPARAM lParam;
 * RETURNS:
 *   (LRESULT) Depends on message.
 */
LRESULT CALLBACK WindowFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;
    POINT pt;
    static INT H, W;
    static HDC hMemDC;
    static HBITMAP hBm;

    switch (Msg)
    {
    case WM_CREATE:
        hDC = GetDC(hWnd);
        hMemDC = CreateCompatibleDC(hDC);
        ReleaseDC(hWnd, hDC);
        SetTimer(hWnd, 30, 1, NULL);
        return 0;
    case WM_SIZE:
        H = HIWORD(lParam);
        W = LOWORD(lParam);
        if (hBm != NULL)
            DeleteObject(hBm);
        hDC = GetDC(hWnd);
        hBm = CreateCompatibleBitmap(hDC, W, H);
        ReleaseDC(hWnd, hDC);
        SelectObject(hMemDC, hBm);
        Ellipse(hMemDC, 0, 0, W, H);
        return 0;
    case WM_RBUTTONDOWN:
        GetCursorPos(&pt);
        ScreenToClient(hWnd, &pt);
        Ellipse(hMemDC, pt.x - 25, pt.y - 25, pt.x + 25, pt.y + 25); 
        return 0;
    case WM_TIMER:
        GetCursorPos(&pt);
        ScreenToClient(hWnd, &pt);
        MoveToEx(hMemDC, 0, 0, NULL);
        LineTo(hMemDC, pt.x, pt.y);
        InvalidateRect(hWnd, NULL, FALSE);
        return 0;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_KEYDOWN:
        if (wParam == 27)
            PostQuitMessage(0);
        return 0;
    case WM_DESTROY:
        DeleteObject(hBm);
        DeleteDC(hMemDC);
        KillTimer(hWnd, 30);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WindowFunc' function */