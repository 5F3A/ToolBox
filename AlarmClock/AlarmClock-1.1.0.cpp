#include <windows.h>
#include <commctrl.h>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

// ---------- 版本信息 ----------
#define APP_VERSION L"1.1.0"
#define APP_NAME L"闹钟设置"

// ---------- 全局变量 ----------
HWND g_hwndMain = NULL;
HWND g_hEditHour, g_hEditMin, g_hEditSec;
HWND g_hBtnSet, g_hBtnCancel, g_hBtnChangelog;

bool g_alarmActive = false;
bool g_alarmTriggered = false;
int g_alarmHour = 0, g_alarmMinute = 0, g_alarmSecond = 0;

// ---------- DPI 感知（使用兼容方式） ----------
void SetDPIAwareness() {
    // 简单的方法：SetProcessDPIAware() 在 Windows Vista 以上可用
    // 虽然不支持 Per-Monitor DPI，但足以保证窗口不模糊
    SetProcessDPIAware();
}

// ---------- 更新日志内容 ----------
wstring GetChangelogText() {
    return L"AlarmClock 更新日志\n"
           L"========================\n"
           L"v1.1.0 (2026-08-31)\n"
           L"  - 支持高 DPI 显示（系统级）\n"
           L"  - 增加“更新日志”按钮，可查看版本历史\n"
           L"  - 使用系统默认字体（现代风格）\n"
           L"\n"
           L"v1.0.0 (2026-08-29)\n"
           L"  - 初始版本，支持设置闹钟时间\n"
           L"  - 闹钟触发后弹出提示\n";
}

// ---------- 窗口过程 ----------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        // 创建控件（不设置自定义字体，使用系统默认）
        CreateWindowW(L"STATIC", L"小时:", WS_CHILD | WS_VISIBLE, 10, 10, 50, 20, hwnd, NULL, NULL, NULL);
        g_hEditHour = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 10, 40, 20, hwnd, NULL, NULL, NULL);
        CreateWindowW(L"STATIC", L"分钟:", WS_CHILD | WS_VISIBLE, 10, 40, 50, 20, hwnd, NULL, NULL, NULL);
        g_hEditMin = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 40, 40, 20, hwnd, NULL, NULL, NULL);
        CreateWindowW(L"STATIC", L"秒:", WS_CHILD | WS_VISIBLE, 10, 70, 50, 20, hwnd, NULL, NULL, NULL);
        g_hEditSec = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 70, 40, 20, hwnd, NULL, NULL, NULL);

        g_hBtnSet = CreateWindowW(L"BUTTON", L"设置", WS_CHILD | WS_VISIBLE, 10, 110, 60, 30, hwnd, (HMENU)1, NULL, NULL);
        g_hBtnCancel = CreateWindowW(L"BUTTON", L"取消", WS_CHILD | WS_VISIBLE, 80, 110, 60, 30, hwnd, (HMENU)2, NULL, NULL);
        g_hBtnChangelog = CreateWindowW(L"BUTTON", L"更新日志", WS_CHILD | WS_VISIBLE, 150, 110, 70, 30, hwnd, (HMENU)3, NULL, NULL);

        // 填入当前时间
        time_t now = time(nullptr);
        tm tm_now;
        localtime_s(&tm_now, &now);
        wchar_t buf[10];
        swprintf(buf, 10, L"%d", tm_now.tm_hour);
        SetWindowTextW(g_hEditHour, buf);
        swprintf(buf, 10, L"%d", tm_now.tm_min);
        SetWindowTextW(g_hEditMin, buf);
        swprintf(buf, 10, L"%d", tm_now.tm_sec);
        SetWindowTextW(g_hEditSec, buf);

        break;
    }
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == 1) { // 设置闹钟
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
            } else {
                MessageBoxW(hwnd, L"无效的时间！", L"错误", MB_OK);
            }
        } else if (id == 2) { // 取消闹钟并退出
            g_alarmActive = false;
            KillTimer(hwnd, 1);
            DestroyWindow(hwnd);
        } else if (id == 3) { // 显示更新日志
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

// ---------- 主入口 ----------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow) {
    // 启用 DPI 感知（系统级）
    SetDPIAwareness();

    // 注册窗口类
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"AlarmClockClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    // 窗口标题包含版本号
    wchar_t title[64];
    wsprintfW(title, L"%s v%s", APP_NAME, APP_VERSION);

    // 创建窗口
    HWND hwnd = CreateWindowW(L"AlarmClockClass", title,
                              WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
                              CW_USEDEFAULT, CW_USEDEFAULT, 250, 190,
                              NULL, NULL, hInstance, NULL);
    if (!hwnd) return 1;
    g_hwndMain = hwnd;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}