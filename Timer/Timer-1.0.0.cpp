#include <windows.h>
#include <commctrl.h>
#include <string>
#include <strsafe.h>
using namespace std;

#define APP_VERSION L"1.0.0"
#define APP_NAME L"计时器"

HWND g_hEditMin, g_hEditSec;
HWND g_hBtnStart, g_hBtnStop;
bool g_running = false;
int g_remainingSec = 0;
HFONT g_hFont = NULL;

void SetDPIAwareness() {
    SetProcessDPIAware();
}

// 根据文本调整按钮宽度
void SetButtonWidthToFitText(HWND hButton, const wchar_t* text) {
    HDC hdc = GetDC(hButton);
    HFONT hFont = (HFONT)SendMessage(hButton, WM_GETFONT, 0, 0);
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont ? hFont : GetStockObject(DEFAULT_GUI_FONT));
    RECT rect = { 0, 0, 0, 0 };
    DrawTextW(hdc, text, -1, &rect, DT_CALCRECT | DT_SINGLELINE);
    int textWidth = rect.right - rect.left;
    SelectObject(hdc, hOldFont);
    ReleaseDC(hButton, hdc);
    int newWidth = textWidth + 12;
    RECT btnRect;
    GetWindowRect(hButton, &btnRect);
    int height = btnRect.bottom - btnRect.top;
    SetWindowPos(hButton, NULL, 0, 0, newWidth, height, SWP_NOMOVE | SWP_NOZORDER);
}

wstring GetChangelogText() {
    return L"Timer 更新日志\n"
        L"========================\n"
        L"v1.0.0 (2026-09-10)\n"
        L"  - 初始版本，支持倒计时\n";
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindowW(L"STATIC", L"分钟:", WS_CHILD | WS_VISIBLE, 10, 10, 50, 20, hwnd, NULL, NULL, NULL);
        g_hEditMin = CreateWindowW(L"EDIT", L"0", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 10, 50, 20, hwnd, NULL, NULL, NULL);
        CreateWindowW(L"STATIC", L"秒:", WS_CHILD | WS_VISIBLE, 10, 40, 50, 20, hwnd, NULL, NULL, NULL);
        g_hEditSec = CreateWindowW(L"EDIT", L"0", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 40, 50, 20, hwnd, NULL, NULL, NULL);

        g_hBtnStart = CreateWindowW(L"BUTTON", L"开始", WS_CHILD | WS_VISIBLE, 10, 80, 60, 30, hwnd, (HMENU)1, NULL, NULL);
        g_hBtnStop = CreateWindowW(L"BUTTON", L"停止", WS_CHILD | WS_VISIBLE, 80, 80, 60, 30, hwnd, (HMENU)2, NULL, NULL);

        SetButtonWidthToFitText(g_hBtnStart, L"开始");
        SetButtonWidthToFitText(g_hBtnStop, L"停止");

        RECT r1, r2;
        GetWindowRect(g_hBtnStart, &r1);
        GetWindowRect(g_hBtnStop, &r2);
        int btnHeight = r1.bottom - r1.top;
        int x = 10;
        SetWindowPos(g_hBtnStart, NULL, x, 80, r1.right - r1.left, btnHeight, SWP_NOZORDER);
        x += (r1.right - r1.left) + 6;
        SetWindowPos(g_hBtnStop, NULL, x, 80, r2.right - r2.left, btnHeight, SWP_NOZORDER);
        break;
    }
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == 1) { // 开始
            if (g_running) return 0;
            wchar_t minStr[10], secStr[10];
            GetWindowTextW(g_hEditMin, minStr, 10);
            GetWindowTextW(g_hEditSec, secStr, 10);
            int min = _wtoi(minStr), sec = _wtoi(secStr);
            if (min < 0 || sec < 0 || sec >= 60) {
                MessageBoxW(hwnd, L"无效的时间！", L"错误", MB_OK);
                return 0;
            }
            g_remainingSec = min * 60 + sec;
            if (g_remainingSec <= 0) {
                MessageBoxW(hwnd, L"时间必须大于 0！", L"错误", MB_OK);
                return 0;
            }
            g_running = true;
            SetTimer(hwnd, 1, 1000, NULL);
            EnableWindow(g_hBtnStart, FALSE);
            EnableWindow(g_hBtnStop, TRUE);
        }
        else if (id == 2) { // 停止
            g_running = false;
            KillTimer(hwnd, 1);
            EnableWindow(g_hBtnStart, TRUE);
            EnableWindow(g_hBtnStop, FALSE);
        }
        break;
    }
    case WM_TIMER: {
        if (g_running) {
            g_remainingSec--;
            if (g_remainingSec <= 0) {
                g_running = false;
                KillTimer(hwnd, 1);
                EnableWindow(g_hBtnStart, TRUE);
                EnableWindow(g_hBtnStop, FALSE);
                MessageBoxW(hwnd, L"时间到！", L"计时器", MB_OK | MB_ICONINFORMATION);
                // 恢复输入
                SetWindowTextW(g_hEditMin, L"0");
                SetWindowTextW(g_hEditSec, L"0");
            }
            else {
                int min = g_remainingSec / 60;
                int sec = g_remainingSec % 60;
                wchar_t buf[10];
                swprintf(buf, 10, L"%d", min);
                SetWindowTextW(g_hEditMin, buf);
                swprintf(buf, 10, L"%d", sec);
                SetWindowTextW(g_hEditSec, buf);
            }
        }
        break;
    }
    case WM_CLOSE: {
        g_running = false;
        KillTimer(hwnd, 1);
        DestroyWindow(hwnd);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow) {
    SetDPIAwareness();

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"TimerClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    wchar_t title[128];
    if (FAILED(StringCchPrintfW(title, 128, L"%s v%s", APP_NAME, APP_VERSION))) {
        wcscpy_s(title, 128, APP_NAME);
    }

    HWND hwnd = CreateWindowW(L"TimerClass", title,
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 220, 160,
        NULL, NULL, hInstance, NULL);
    if (!hwnd) return 1;
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}