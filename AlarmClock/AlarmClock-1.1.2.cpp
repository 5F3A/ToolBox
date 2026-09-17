#include <windows.h>
#include <commctrl.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include <strsafe.h>
using namespace std;

// ---------- 版本信息 ----------
#define APP_VERSION L"1.1.2"
#define APP_NAME L"闹钟设置"

// ---------- 全局变量 ----------
HWND g_hwndMain = NULL;
HWND g_hEditHour, g_hEditMin, g_hEditSec;
HWND g_hBtnSet, g_hBtnCancel, g_hBtnChangelog;

bool g_alarmActive = false;
bool g_alarmTriggered = false;
int g_alarmHour = 0, g_alarmMinute = 0, g_alarmSecond = 0;

// 命令行参数的默认值（-1 表示使用当前时间）
int g_defaultHour = -1, g_defaultMinute = -1, g_defaultSecond = -1;

// ---------- DPI 感知 ----------
void SetDPIAwareness() {
    SetProcessDPIAware();
}

// ---------- 根据文本调整按钮宽度 ----------
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

// ---------- 更新日志 ----------
wstring GetChangelogText() {
    return L"AlarmClock 更新日志\n"
        L"========================\n"
        L"v1.1.2 (2026-09-16)\n"
        L"  - 增加命令行参数填充：AlarmClock.exe HH MM SS\n"
        L"  - 集成调试信息支持\n"
        L"\n"
        L"v1.1.1 (2026-08-31)\n"
        L"  - 修复标题显示错误\n"
        L"  - 修复按钮文本显示不全问题\n"
        L"\n"
        L"v1.1.0 (2026-08-31)\n"
        L"  - 支持高 DPI 显示\n"
        L"  - 增加\"更新日志\"按钮\n"
        L"\n"
        L"v1.0.0 (2026-08-29)\n"
        L"  - 初始版本\n";
}

// ---------- 窗口过程 ----------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindowW(L"STATIC", L"小时:", WS_CHILD | WS_VISIBLE, 10, 10, 50, 20, hwnd, NULL, NULL, NULL);
        g_hEditHour = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 10, 40, 20, hwnd, NULL, NULL, NULL);
        CreateWindowW(L"STATIC", L"分钟:", WS_CHILD | WS_VISIBLE, 10, 40, 50, 20, hwnd, NULL, NULL, NULL);
        g_hEditMin = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 40, 40, 20, hwnd, NULL, NULL, NULL);
        CreateWindowW(L"STATIC", L"秒:", WS_CHILD | WS_VISIBLE, 10, 70, 50, 20, hwnd, NULL, NULL, NULL);
        g_hEditSec = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 70, 40, 20, hwnd, NULL, NULL, NULL);

        g_hBtnSet = CreateWindowW(L"BUTTON", L"设置", WS_CHILD | WS_VISIBLE, 10, 110, 60, 30, hwnd, (HMENU)1, NULL, NULL);
        g_hBtnCancel = CreateWindowW(L"BUTTON", L"取消", WS_CHILD | WS_VISIBLE, 80, 110, 60, 30, hwnd, (HMENU)2, NULL, NULL);
        g_hBtnChangelog = CreateWindowW(L"BUTTON", L"更新日志", WS_CHILD | WS_VISIBLE, 150, 110, 70, 30, hwnd, (HMENU)3, NULL, NULL);

        SetButtonWidthToFitText(g_hBtnSet, L"设置");
        SetButtonWidthToFitText(g_hBtnCancel, L"取消");
        SetButtonWidthToFitText(g_hBtnChangelog, L"更新日志");

        RECT rectSet, rectCancel, rectChangelog;
        GetWindowRect(g_hBtnSet, &rectSet);
        GetWindowRect(g_hBtnCancel, &rectCancel);
        GetWindowRect(g_hBtnChangelog, &rectChangelog);
        int btnHeight = rectSet.bottom - rectSet.top;
        int x = 10;
        SetWindowPos(g_hBtnSet, NULL, x, 110, rectSet.right - rectSet.left, btnHeight, SWP_NOZORDER);
        x += (rectSet.right - rectSet.left) + 6;
        SetWindowPos(g_hBtnCancel, NULL, x, 110, rectCancel.right - rectCancel.left, btnHeight, SWP_NOZORDER);
        x += (rectCancel.right - rectCancel.left) + 6;
        SetWindowPos(g_hBtnChangelog, NULL, x, 110, rectChangelog.right - rectChangelog.left, btnHeight, SWP_NOZORDER);

        // === 关键：优先使用命令行参数，否则用当前时间 ===
        int h, m, s;
        if (g_defaultHour >= 0) {
            h = g_defaultHour;
            m = g_defaultMinute;
            s = g_defaultSecond;
        }
        else {
            time_t now = time(nullptr);
            tm tm_now;
            localtime_s(&tm_now, &now);
            h = tm_now.tm_hour;
            m = tm_now.tm_min;
            s = tm_now.tm_sec;
        }
        wchar_t buf[10];
        swprintf(buf, 10, L"%d", h);
        SetWindowTextW(g_hEditHour, buf);
        swprintf(buf, 10, L"%d", m);
        SetWindowTextW(g_hEditMin, buf);
        swprintf(buf, 10, L"%d", s);
        SetWindowTextW(g_hEditSec, buf);
        break;
    }
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == 1) {
            wchar_t hStr[10], mStr[10], sStr[10];
            GetWindowTextW(g_hEditHour, hStr, 10);
            GetWindowTextW(g_hEditMin, mStr, 10);
            GetWindowTextW(g_hEditSec, sStr, 10);
            int h = _wtoi(hStr), m = _wtoi(mStr), s = _wtoi(sStr);
            if (h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60) {
                g_alarmHour = h; g_alarmMinute = m; g_alarmSecond = s;
                g_alarmActive = true; g_alarmTriggered = false;
                MessageBoxW(hwnd, L"闹钟已设置！", L"信息", MB_OK);
                ShowWindow(hwnd, SW_MINIMIZE);
                SetTimer(hwnd, 1, 1000, NULL);
            }
            else {
                MessageBoxW(hwnd, L"无效的时间！", L"错误", MB_OK);
            }
        }
        else if (id == 2) {
            g_alarmActive = false;
            KillTimer(hwnd, 1);
            DestroyWindow(hwnd);
        }
        else if (id == 3) {
            MessageBoxW(hwnd, GetChangelogText().c_str(), L"更新日志", MB_OK | MB_ICONINFORMATION);
        }
        break;
    }
    case WM_TIMER: {
        if (g_alarmActive && !g_alarmTriggered) {
            time_t now = time(nullptr);
            tm tm_now;
            localtime_s(&tm_now, &now);
            if (tm_now.tm_hour == g_alarmHour &&
                tm_now.tm_min == g_alarmMinute &&
                tm_now.tm_sec == g_alarmSecond) {
                g_alarmTriggered = true; g_alarmActive = false;
                KillTimer(hwnd, 1);
                MessageBoxW(hwnd, L"时间到了！", L"闹钟", MB_OK | MB_ICONINFORMATION);
                ShowWindow(hwnd, SW_RESTORE);
                SetForegroundWindow(hwnd);
            }
        }
        break;
    }
    case WM_CLOSE: {
        g_alarmActive = false;
        KillTimer(hwnd, 1);
        DestroyWindow(hwnd);
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow) {
    SetDPIAwareness();

    // === 解析命令行参数：AlarmClock.exe HH MM SS ===
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argv) {
        if (argc >= 4) {
            int h = _wtoi(argv[1]);
            int m = _wtoi(argv[2]);
            int s = _wtoi(argv[3]);
            if (h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60) {
                g_defaultHour = h;
                g_defaultMinute = m;
                g_defaultSecond = s;
            }
        }
        LocalFree(argv);
    }

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"AlarmClockClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    wchar_t title[128];
    if (FAILED(StringCchPrintfW(title, 128, L"%s v%s", APP_NAME, APP_VERSION))) {
        wcscpy_s(title, 128, APP_NAME);
    }

    HWND hwnd = CreateWindowW(L"AlarmClockClass", title,
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 350, 190,
        NULL, NULL, hInstance, NULL);
    if (!hwnd) return 1;
    g_hwndMain = hwnd;

    SetWindowTextW(hwnd, title);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}