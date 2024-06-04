#include <windows.h>
#include <stdio.h>
#define BUTTON_ID 1

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int count = 0;
HWND hButton;
HWND hCount;
char countText[10];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Counter App",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            hButton = CreateWindow(
                "BUTTON",
                "Click Me",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                50,
                50,
                100,
                30,
                hwnd,
                (HMENU)BUTTON_ID,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );
            hCount = CreateWindow(
                "STATIC",
                "0",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                50,
                100,
                100,
                30,
                hwnd,
                NULL,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == BUTTON_ID) {
                count++;
                sprintf(countText, "%d", count);
                SetWindowText(hCount, countText);
                if (count == 10) {
                    HWND hWinWindow = CreateWindowEx(
                        0,
                        "WinClass",
                        "Congratulations",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
                        NULL,
                        NULL,
                        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                        NULL
                    );
                    ShowWindow(hWinWindow, SW_SHOW);
                }
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WinWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            SetBkColor(hdc, RGB(0, 0, 0));
            SetTextColor(hdc, RGB(255, 0, 0));
            RECT rect;
            GetClientRect(hwnd, &rect);
            DrawText(hdc, "You Win!", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

ATOM RegisterWinWindowClass(HINSTANCE hInstance) {
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WinWindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "WinClass";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    return RegisterClass(&wc);
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    RegisterWinWindowClass(hInstance);

    return WinMain(hInstance, NULL, GetCommandLineA(), SW_SHOWNORMAL);
}
