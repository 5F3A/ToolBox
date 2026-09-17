#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

// ---------- 版本号定义 ----------
#define APP_VERSION L"1.0.0"

HWND hEditHour, hEditMin, hEditSec;
bool g_alarmActive = false, g_alarmTriggered = false;
int g_alarmHour = 0, g_alarmMinute = 0, g_alarmSecond = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindowW(L"STATIC", L"小时:", WS_CHILD | WS_VISIBLE, 10, 10, 50, 20, hwnd, NULL, NULL, NULL);
        hEditHour = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 10, 40, 20, hwnd, NULL, NULL, NULL);
        CreateWindowW(L"STATIC", L"分钟:", WS_CHILD | WS_VISIBLE, 10, 40, 50, 20, hwnd, NULL, NULL, NULL);
        hEditMin = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 40, 40, 20, hwnd, NULL, NULL, NULL);
        CreateWindowW(L"STATIC", L"秒:", WS_CHILD | WS_VISIBLE, 10, 70, 50, 20, hwnd, NULL, NULL, NULL);
        hEditSec = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 70, 40, 20, hwnd, NULL, NULL, NULL);
        CreateWindowW(L"BUTTON", L"设置", WS_CHILD | WS_VISIBLE, 10, 110, 80, 30, hwnd, (HMENU)1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"取消", WS_CHILD | WS_VISIBLE, 100, 110, 80, 30, hwnd, (HMENU)2, NULL, NULL);

        time_t now = time(nullptr);
        tm tm_now;
        localtime_s(&tm_now, &now);
        wchar_t buf[10];
        swprintf(buf, 10, L"%d", tm_now.tm_hour);
        SetWindowTextW(hEditHour, buf);
        swprintf(buf, 10, L"%d", tm_now.tm_min);
        SetWindowTextW(hEditMin, buf);
        swprintf(buf, 10, L"%d", tm_now.tm_sec);
        SetWindowTextW(hEditSec, buf);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            wchar_t hStr[10], mStr[10], sStr[10];
            GetWindowTextW(hEditHour, hStr, 10);
            GetWindowTextW(hEditMin, mStr, 10);
            GetWindowTextW(hEditSec, sStr, 10);
            int h = _wtoi(hStr), m = _wtoi(mStr), s = _wtoi(sStr);
            if (h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60) {
                g_alarmHour = h; g_alarmMinute = m; g_alarmSecond = s;
                g_alarmActive = true; g_alarmTriggered = false;
                MessageBoxW(hwnd, L"闹钟已设置！", L"信息", MB_OK);
                ShowWindow(hwnd, SW_MINIMIZE);
                SetTimer(hwnd, 1, 1000, NULL);
            } else MessageBoxW(hwnd, L"无效的时间！", L"错误", MB_OK);
        } else if (LOWORD(wParam) == 2) {
            g_alarmActive = false;
            KillTimer(hwnd, 1);
            DestroyWindow(hwnd);
        }
        break;
    case WM_TIMER:
        if (g_alarmActive && !g_alarmTriggered) {
            time_t now = time(nullptr);
            tm tm_now;
            localtime_s(&tm_now, &now);
            if (tm_now.tm_hour == g_alarmHour && tm_now.tm_min == g_alarmMinute && tm_now.tm_sec == g_alarmSecond) {
                g_alarmTriggered = true; g_alarmActive = false;
                KillTimer(hwnd, 1);
                MessageBoxW(hwnd, L"⏰ 时间到了！", L"闹钟", MB_OK | MB_ICONINFORMATION);
                ShowWindow(hwnd, SW_RESTORE);
                SetForegroundWindow(hwnd);
            }
        }
        break;
    case WM_CLOSE:
        g_alarmActive = false;
        KillTimer(hwnd, 1);
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow) {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"AlarmClockClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    // ----- 窗口标题包含版本号 -----
    wchar_t title[64];
    wsprintfW(title, L"闹钟设置 v%s", APP_VERSION);

    HWND hwnd = CreateWindowW(L"AlarmClockClass", title,
                              WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
                              CW_USEDEFAULT, CW_USEDEFAULT, 220, 200,
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