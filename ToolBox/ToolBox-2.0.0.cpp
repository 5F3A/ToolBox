#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <conio.h>
#include <thread>
#include <chrono>
#include <filesystem>
using namespace std;

// ============ 多语言支持 ============
enum TextID {
    TXT_LOADING, TXT_WELCOME, TXT_PROMPT, TXT_HELP_HEADER, TXT_HELP_UPDLOG,
    TXT_HELP_VER, TXT_HELP_HELP, TXT_HELP_EXIT, TXT_HELP_EXITF, TXT_HELP_EXITF_DESC,
    TXT_HELP_ABORTF, TXT_HELP_CRASH, TXT_HELP_CMD, TXT_HELP_MKE, TXT_HELP_RF,
    TXT_HELP_SHUTDOWN, TXT_HELP_RESTART, TXT_HELP_SLEEP, TXT_HELP_FW, TXT_HELP_REC,
    TXT_HELP_RUN, TXT_HELP_CLS, TXT_HELP_PROG1, TXT_HELP_PROG2, TXT_HELP_PROG3,
    TXT_HELP_PROG4, TXT_HELP_PROG5, TXT_HELP_PROGB1, TXT_HELP_PROGB2, TXT_HELP_DEBUG,
    TXT_HELP_UNDEBUG, TXT_HELP_PROGRAM_RUN_HEADER, TXT_HELP_DEBUG_HEADER,
    TXT_VER_INFO, TXT_UPDLOG_TITLE, TXT_UPDLOG_CONTENT, TXT_MKE_CONTENT,
    TXT_CMD_UNKNOWN, TXT_EXITF_INVALID, TXT_EXITF_OUTRANGE, TXT_EXITF_ERROR,
    TXT_EXITF_UNKNOWN, TXT_FILE_PATH_EMPTY, TXT_FILE_OPEN_FAIL, TXT_PROGRAM_NAME_EMPTY,
    TXT_PROGRAM_NAME_ILLEGAL, TXT_PROGRAM_NAME_SPACE, TXT_RUN_PROMPT, TXT_SHUTDOWN_MSG,
    TXT_RESTART_MSG, TXT_SLEEP_MSG, TXT_FW_MSG, TXT_REC_MSG, TXT_RETURN_VALUE,
    TXT_ARGS_HEADER, TXT_ARGS_ITEM, TXT_NO_ARGS, TXT_CRASH_FAIL, TXT_SWITCHING_CP,
    TXT_LANG_CHANGED, TXT_LANG_UNSUPPORTED, TXT_LANG_PROMPT, TXT_HELP_LANG,
    TXT_HELP_TIME, TXT_HELP_LOOPTIME, TXT_HELP_DATE, TXT_HELP_ALARM,
    TXT_ALARM_LAUNCH, TXT_TIME_NOW, TXT_DATE_NOW, TXT_LOOPTIME_EXIT,
    TXT_HELP_JIYUKILL, TXT_JIYUKILL_LAUNCH,
    TXT_HELP_TIMER, TXT_TIMER_LAUNCH, TXT_TIMER_NOT_FOUND,
    TXT_HELP_BADCRASH, TXT_BADCRASH_MSG,
    TXT_HELP_EMERGENCY, TXT_EMERGENCY_MSG,
    TXT_HELP_MEMORY, TXT_MEMORY_HEADER, TXT_MEMORY_ERROR,
    TXT_HELP_REPAIR, TXT_REPAIR_MSG,
    TXT_COUNT
};

const int LANG_ZH_CN = 0;
const int LANG_EN_US = 10000;
string texts[500000] = {};
string g_currentLang = "zh-CN";

int GetLangOffset() {
    return (g_currentLang == "en-US") ? LANG_EN_US : LANG_ZH_CN;
}
string GetText(TextID id) { return texts[id + GetLangOffset()]; }

// ============ RAII 计时器 ============
struct ScopeTimer {
    chrono::steady_clock::time_point start;
    bool enabled;
    ScopeTimer(bool en) : enabled(en) {
        if (enabled) start = chrono::steady_clock::now();
    }
    ~ScopeTimer() {
        if (enabled) {
            auto us = chrono::duration_cast<chrono::microseconds>(
                chrono::steady_clock::now() - start).count();
            cout << "[DEBUG] Command handled in " << (us / 1000.0) << " ms" << endl;
        }
    }
};

// BADCRASH 递归函数
__declspec(noinline) static void BadCrashRecursive() {
    volatile char buf[8192];
    buf[0] = 'C';
    BadCrashRecursive();
}

void InitTexts() {
    // ===== 中文 =====
    texts[TXT_LOADING + LANG_ZH_CN] = "加载中......";
    texts[TXT_WELCOME + LANG_ZH_CN] = "欢迎使用工具箱！输入 help 查看帮助。";
    texts[TXT_PROMPT + LANG_ZH_CN] = "请输入指令 > ";
    texts[TXT_HELP_HEADER + LANG_ZH_CN] = "-------------------- HELP-MENU -----------------";
    texts[TXT_HELP_UPDLOG + LANG_ZH_CN] = "UPDLOG   更新日志";
    texts[TXT_HELP_VER + LANG_ZH_CN] = "VER      查看版本";
    texts[TXT_HELP_HELP + LANG_ZH_CN] = "HELP     查看帮助";
    texts[TXT_HELP_EXIT + LANG_ZH_CN] = "EXIT     退出程序";
    texts[TXT_HELP_EXITF + LANG_ZH_CN] = "EXITF    使用 exit() 退出程序";
    texts[TXT_HELP_EXITF_DESC + LANG_ZH_CN] = "EXITF xx 使用 exit() 退出程序，退出代码为 xx";
    texts[TXT_HELP_ABORTF + LANG_ZH_CN] = "ABORTF   使用 abort() 退出程序";
    texts[TXT_HELP_CRASH + LANG_ZH_CN] = "CRASH    以 nullptr 崩溃程序";
    texts[TXT_HELP_CMD + LANG_ZH_CN] = "CMD      启动 CMD";
    texts[TXT_HELP_MKE + LANG_ZH_CN] = "MKE      显示作者";
    texts[TXT_HELP_RF + LANG_ZH_CN] = "RF       查看运行参数";
    texts[TXT_HELP_SHUTDOWN + LANG_ZH_CN] = "SHUTDOWN 关机";
    texts[TXT_HELP_RESTART + LANG_ZH_CN] = "RESTART  重启";
    texts[TXT_HELP_SLEEP + LANG_ZH_CN] = "SLEEP    睡眠";
    texts[TXT_HELP_FW + LANG_ZH_CN] = "FW       转到固件(BIOS/UEFI)";
    texts[TXT_HELP_REC + LANG_ZH_CN] = "REC      转到恢复环境(WinRE)";
    texts[TXT_HELP_RUN + LANG_ZH_CN] = "RUN      执行本工具目录下的程序";
    texts[TXT_HELP_CLS + LANG_ZH_CN] = "CLS      清除屏幕";
    texts[TXT_HELP_LANG + LANG_ZH_CN] = "LANG     切换语言";
    texts[TXT_HELP_PROGRAM_RUN_HEADER + LANG_ZH_CN] = "------------------ PROGRAM RUN  ----------------";
    texts[TXT_HELP_PROG1 + LANG_ZH_CN] = "PROG1    执行本工具目录下的 PROGRAM1.EXE";
    texts[TXT_HELP_PROG2 + LANG_ZH_CN] = "PROG2    执行本工具目录下的 PROGRAM2.EXE";
    texts[TXT_HELP_PROG3 + LANG_ZH_CN] = "PROG3    执行本工具目录下的 PROGRAM3.EXE";
    texts[TXT_HELP_PROG4 + LANG_ZH_CN] = "PROG4    执行本工具目录下的 PROGRAM4.EXE";
    texts[TXT_HELP_PROG5 + LANG_ZH_CN] = "PROG5    执行本工具目录下的 PROGRAM5.EXE";
    texts[TXT_HELP_PROGB1 + LANG_ZH_CN] = "PROGB1   执行本工具目录下的 PROGRAM1.BAT";
    texts[TXT_HELP_PROGB2 + LANG_ZH_CN] = "PROGB2   执行本工具目录下的 PROGRAM2.BAT";
    texts[TXT_HELP_DEBUG_HEADER + LANG_ZH_CN] = "---------------------- DEBUG -------------------";
    texts[TXT_HELP_DEBUG + LANG_ZH_CN] = "DEBUG    打开调试模式（DEBUG HELP 查看子命令）";
    texts[TXT_HELP_UNDEBUG + LANG_ZH_CN] = "UNDEBUG  关闭调试模式";
    texts[TXT_HELP_TIME + LANG_ZH_CN] = "TIME     查看当前时间";
    texts[TXT_HELP_LOOPTIME + LANG_ZH_CN] = "LOOPTIME 循环显示时间（按任意键退出）";
    texts[TXT_HELP_DATE + LANG_ZH_CN] = "DATE     查看当前日期";
    texts[TXT_HELP_ALARM + LANG_ZH_CN] = "ALARM    打开闹钟设置（独立 GUI）";
    texts[TXT_HELP_JIYUKILL + LANG_ZH_CN] = "JIYUKILL / JYKILL  执行 JiYuKiller.exe 杀死 StudentMain.exe 进程";
    texts[TXT_HELP_TIMER + LANG_ZH_CN] = "TIMER    打开计时器（独立 GUI）";
    texts[TXT_HELP_BADCRASH + LANG_ZH_CN] = "BADCRASH 触发栈溢出（严重崩溃）";
    texts[TXT_HELP_EMERGENCY + LANG_ZH_CN] = "EMERGENCY 立即强制关机（不等待）";
    texts[TXT_HELP_MEMORY + LANG_ZH_CN] = "MEMORY   查看物理内存使用情况";
    texts[TXT_HELP_REPAIR + LANG_ZH_CN] = "REPAIR   运行系统文件检查器（sfc /scannow）";
    texts[TXT_BADCRASH_MSG + LANG_ZH_CN] = "触发栈溢出崩溃... 程序将立即终止。";
    texts[TXT_EMERGENCY_MSG + LANG_ZH_CN] = "【紧急】立即强制关机！";
    texts[TXT_MEMORY_HEADER + LANG_ZH_CN] = "===== 内存信息 =====";
    texts[TXT_MEMORY_ERROR + LANG_ZH_CN] = "获取内存信息失败！";
    texts[TXT_REPAIR_MSG + LANG_ZH_CN] = "正在运行系统文件检查器（可能需要管理员权限）...";
    texts[TXT_VER_INFO + LANG_ZH_CN] = "Tool-Box 2.0.0-snapshot6";
    texts[TXT_UPDLOG_TITLE + LANG_ZH_CN] = "更新日志：";
    texts[TXT_UPDLOG_CONTENT + LANG_ZH_CN] = R"(
2026-08-06
Dev 阶段: 已经跳过
Alpha 阶段: 已经跳过
Beta 阶段: 已经跳过
Pre-Release 阶段: 已经丢失
1.0.0: 正式版发布
1.1.0: 增加 RF 指令
1.2.0: 增加 UPDLOG 指令
1.2.1: 添加文件有效性检查
1.2.2: 移除 DBZ
1.3.0: 输入参数功能成功
1.4.0: 加入 MKE
1.5.0: 使用 getline()
1.5.1: 修复空指令
1.6.0: RUN 指令
1.7.0: SHUTDOWN / RESTART
1.8.0: CLS
1.9.0: SLEEP / FW / REC
1.9.1: 更改 UPDLOG 格式
1.10.0: UPDF
2.0.0-beta: 多语言
2.0.0-snapshot1: Nothings
2026-08-30
2.0.0-snapshot2: TIME / LOOPTIME / DATE / ALARM
2026-08-31
2.0.0-snapshot3: ALARM 自动查找，JIYUKILL
2026-09-10
2.0.0-snapshot5: TIMER 指令
2026-09-16
2.0.0-snapshot6: 新增 BADCRASH / EMERGENCY / MEMORY / REPAIR 指令
                  增强 DEBUG：子命令、token 分析、耗时统计
)";
    texts[TXT_MKE_CONTENT + LANG_ZH_CN] = R"(
作者：
YuanMT（主作者 | Bilibili: https://space.bilibili.com/3546939977107567）
DeepSeek v4&r1（协助 | Address: https://www.deepseek.com）
Visual Studio AI（协助）
TT一TT（网站 | https://tt-web.gt.tc）
如果你想要在此处看到你的名字，请至少协助 1 个版本的开发
)";
    texts[TXT_CMD_UNKNOWN + LANG_ZH_CN] = "无效的指令！";
    texts[TXT_EXITF_INVALID + LANG_ZH_CN] = "无效的退出代码！请确保输入的是一个整数。";
    texts[TXT_EXITF_OUTRANGE + LANG_ZH_CN] = "退出代码超出范围！";
    texts[TXT_EXITF_ERROR + LANG_ZH_CN] = "发生错误：";
    texts[TXT_EXITF_UNKNOWN + LANG_ZH_CN] = "发生未知错误！";
    texts[TXT_FILE_PATH_EMPTY + LANG_ZH_CN] = "文件路径为空！";
    texts[TXT_FILE_OPEN_FAIL + LANG_ZH_CN] = "文件打开失败！请检查文件是否存在。";
    texts[TXT_PROGRAM_NAME_EMPTY + LANG_ZH_CN] = "程序名称为空！";
    texts[TXT_PROGRAM_NAME_ILLEGAL + LANG_ZH_CN] = "程序名称包含非法字符！";
    texts[TXT_PROGRAM_NAME_SPACE + LANG_ZH_CN] = "程序名称包含空格或制表符！";
    texts[TXT_RUN_PROMPT + LANG_ZH_CN] = "请输入要执行的程序名称（包含后缀名）：";
    texts[TXT_SHUTDOWN_MSG + LANG_ZH_CN] = "正在关机……";
    texts[TXT_RESTART_MSG + LANG_ZH_CN] = "正在重启……";
    texts[TXT_SLEEP_MSG + LANG_ZH_CN] = "正在睡眠……";
    texts[TXT_FW_MSG + LANG_ZH_CN] = "正在进入固件(BIOS/UEFI)……";
    texts[TXT_REC_MSG + LANG_ZH_CN] = "正在进入恢复环境(WinRE)……";
    texts[TXT_RETURN_VALUE + LANG_ZH_CN] = "返回值：";
    texts[TXT_ARGS_HEADER + LANG_ZH_CN] = "程序启动参数（共 %d 个）：";
    texts[TXT_ARGS_ITEM + LANG_ZH_CN] = "  args[%d] = %s";
    texts[TXT_NO_ARGS + LANG_ZH_CN] = "  （无参数）";
    texts[TXT_CRASH_FAIL + LANG_ZH_CN] = "崩溃失败……";
    texts[TXT_SWITCHING_CP + LANG_ZH_CN] = "Switching Code Page......\nPlease wait......\n";
    texts[TXT_LANG_CHANGED + LANG_ZH_CN] = "语言已切换为 %s";
    texts[TXT_LANG_UNSUPPORTED + LANG_ZH_CN] = "不支持的语言。";
    texts[TXT_LANG_PROMPT + LANG_ZH_CN] = "当前语言：%s\n可用语言：zh-CN, en-US\n请输入语言代码：";
    texts[TXT_ALARM_LAUNCH + LANG_ZH_CN] = "正在启动闹钟程序";
    texts[TXT_TIME_NOW + LANG_ZH_CN] = "当前时间：";
    texts[TXT_DATE_NOW + LANG_ZH_CN] = "当前日期：";
    texts[TXT_LOOPTIME_EXIT + LANG_ZH_CN] = "按任意键退出循环……";
    texts[TXT_JIYUKILL_LAUNCH + LANG_ZH_CN] = "正在启动 JiYuKiller.exe 终止 StudentMain.exe...";
    texts[TXT_TIMER_LAUNCH + LANG_ZH_CN] = "正在启动计时器程序";
    texts[TXT_TIMER_NOT_FOUND + LANG_ZH_CN] = "错误：找不到 Timer-*.exe 文件。";

    // ===== 英文 =====
    texts[TXT_LOADING + LANG_EN_US] = "Loading......";
    texts[TXT_WELCOME + LANG_EN_US] = "Welcome to Toolbox! Type help for help.";
    texts[TXT_PROMPT + LANG_EN_US] = "Enter command > ";
    texts[TXT_HELP_HEADER + LANG_EN_US] = "-------------------- HELP-MENU -----------------";
    texts[TXT_HELP_UPDLOG + LANG_EN_US] = "UPDLOG   Update log";
    texts[TXT_HELP_VER + LANG_EN_US] = "VER      View version";
    texts[TXT_HELP_HELP + LANG_EN_US] = "HELP     Show help";
    texts[TXT_HELP_EXIT + LANG_EN_US] = "EXIT     Exit program";
    texts[TXT_HELP_EXITF + LANG_EN_US] = "EXITF    Exit using exit()";
    texts[TXT_HELP_EXITF_DESC + LANG_EN_US] = "EXITF xx Exit with code xx";
    texts[TXT_HELP_ABORTF + LANG_EN_US] = "ABORTF   Exit using abort()";
    texts[TXT_HELP_CRASH + LANG_EN_US] = "CRASH    Crash with nullptr";
    texts[TXT_HELP_CMD + LANG_EN_US] = "CMD      Launch CMD";
    texts[TXT_HELP_MKE + LANG_EN_US] = "MKE      Show author";
    texts[TXT_HELP_RF + LANG_EN_US] = "RF       Show run parameters";
    texts[TXT_HELP_SHUTDOWN + LANG_EN_US] = "SHUTDOWN Shutdown";
    texts[TXT_HELP_RESTART + LANG_EN_US] = "RESTART  Restart";
    texts[TXT_HELP_SLEEP + LANG_EN_US] = "SLEEP    Sleep";
    texts[TXT_HELP_FW + LANG_EN_US] = "FW       Enter firmware (BIOS/UEFI)";
    texts[TXT_HELP_REC + LANG_EN_US] = "REC      Enter recovery (WinRE)";
    texts[TXT_HELP_RUN + LANG_EN_US] = "RUN      Run a program in this directory";
    texts[TXT_HELP_CLS + LANG_EN_US] = "CLS      Clear screen";
    texts[TXT_HELP_LANG + LANG_EN_US] = "LANG     Change language";
    texts[TXT_HELP_PROGRAM_RUN_HEADER + LANG_EN_US] = "------------------ PROGRAM RUN  ----------------";
    texts[TXT_HELP_PROG1 + LANG_EN_US] = "PROG1    Run PROGRAM1.EXE";
    texts[TXT_HELP_PROG2 + LANG_EN_US] = "PROG2    Run PROGRAM2.EXE";
    texts[TXT_HELP_PROG3 + LANG_EN_US] = "PROG3    Run PROGRAM3.EXE";
    texts[TXT_HELP_PROG4 + LANG_EN_US] = "PROG4    Run PROGRAM4.EXE";
    texts[TXT_HELP_PROG5 + LANG_EN_US] = "PROG5    Run PROGRAM5.EXE";
    texts[TXT_HELP_PROGB1 + LANG_EN_US] = "PROGB1   Run PROGRAM1.BAT";
    texts[TXT_HELP_PROGB2 + LANG_EN_US] = "PROGB2   Run PROGRAM2.BAT";
    texts[TXT_HELP_DEBUG_HEADER + LANG_EN_US] = "---------------------- DEBUG -------------------";
    texts[TXT_HELP_DEBUG + LANG_EN_US] = "DEBUG    Enable debug mode (DEBUG HELP for subcommands)";
    texts[TXT_HELP_UNDEBUG + LANG_EN_US] = "UNDEBUG  Disable debug mode";
    texts[TXT_HELP_TIME + LANG_EN_US] = "TIME     Show current time";
    texts[TXT_HELP_LOOPTIME + LANG_EN_US] = "LOOPTIME Loop show time (press any key to exit)";
    texts[TXT_HELP_DATE + LANG_EN_US] = "DATE     Show current date";
    texts[TXT_HELP_ALARM + LANG_EN_US] = "ALARM    Open alarm GUI (standalone)";
    texts[TXT_HELP_JIYUKILL + LANG_EN_US] = "JIYUKILL / JYKILL  Run JiYuKiller.exe to kill StudentMain.exe";
    texts[TXT_HELP_TIMER + LANG_EN_US] = "TIMER    Open timer GUI (standalone)";
    texts[TXT_HELP_BADCRASH + LANG_EN_US] = "BADCRASH Trigger stack overflow (severe crash)";
    texts[TXT_HELP_EMERGENCY + LANG_EN_US] = "EMERGENCY Immediate forced shutdown";
    texts[TXT_HELP_MEMORY + LANG_EN_US] = "MEMORY   Show physical memory usage";
    texts[TXT_HELP_REPAIR + LANG_EN_US] = "REPAIR   Run System File Checker (sfc /scannow)";
    texts[TXT_BADCRASH_MSG + LANG_EN_US] = "Triggering stack overflow... program will terminate immediately.";
    texts[TXT_EMERGENCY_MSG + LANG_EN_US] = "[EMERGENCY] Immediate forced shutdown!";
    texts[TXT_MEMORY_HEADER + LANG_EN_US] = "===== Memory Information =====";
    texts[TXT_MEMORY_ERROR + LANG_EN_US] = "Failed to get memory information!";
    texts[TXT_REPAIR_MSG + LANG_EN_US] = "Running System File Checker (may require admin rights)...";
    texts[TXT_VER_INFO + LANG_EN_US] = "Tool-Box 2.0.0-snapshot6";
    texts[TXT_UPDLOG_TITLE + LANG_EN_US] = "Update log:";
    texts[TXT_UPDLOG_CONTENT + LANG_EN_US] = R"(
2026-08-06
Dev phase: skipped
Alpha phase: skipped
Beta phase: skipped
Pre-Release phase: lost
1.0.0: Official release
1.1.0: Added RF
1.2.0: Added UPDLOG
1.2.1: Added file validation
1.2.2: Removed DBZ
1.3.0: Input parameter feature succeeded
1.4.0: Added MKE
1.5.0: Used getline()
1.5.1: Fixed empty command
1.6.0: RUN command
1.7.0: SHUTDOWN / RESTART
1.8.0: CLS
1.9.0: SLEEP / FW / REC
1.9.1: Changed UPDLOG format
1.10.0: UPDF
2.0.0-beta: Multi-language
2.0.0-snapshot1: Nothings
2026-08-30
2.0.0-snapshot2: TIME / LOOPTIME / DATE / ALARM
2026-08-31
2.0.0-snapshot3: ALARM auto-find, JIYUKILL
2026-09-10
2.0.0-snapshot5: TIMER command
2026-09-16
2.0.0-snapshot6: Added BADCRASH / EMERGENCY / MEMORY / REPAIR
                  Enhanced DEBUG: subcommands, token analysis, timing
)";
    texts[TXT_MKE_CONTENT + LANG_EN_US] = R"(
Author:
YuanMT (Main author | Bilibili: https://space.bilibili.com/3546939977107567)
DeepSeek v4&r1 (Assistance | https://www.deepseek.com)
Visual Studio AI (Assistance)
TT一TT (Website | https://tt-web.gt.tc)
)";
    texts[TXT_CMD_UNKNOWN + LANG_EN_US] = "Unknown command!";
    texts[TXT_EXITF_INVALID + LANG_EN_US] = "Invalid exit code!";
    texts[TXT_EXITF_OUTRANGE + LANG_EN_US] = "Exit code out of range!";
    texts[TXT_EXITF_ERROR + LANG_EN_US] = "Error: ";
    texts[TXT_EXITF_UNKNOWN + LANG_EN_US] = "Unknown error!";
    texts[TXT_FILE_PATH_EMPTY + LANG_EN_US] = "File path is empty!";
    texts[TXT_FILE_OPEN_FAIL + LANG_EN_US] = "File open failed!";
    texts[TXT_PROGRAM_NAME_EMPTY + LANG_EN_US] = "Program name is empty!";
    texts[TXT_PROGRAM_NAME_ILLEGAL + LANG_EN_US] = "Program name contains illegal characters!";
    texts[TXT_PROGRAM_NAME_SPACE + LANG_EN_US] = "Program name contains spaces or tabs!";
    texts[TXT_RUN_PROMPT + LANG_EN_US] = "Enter program name (with extension): ";
    texts[TXT_SHUTDOWN_MSG + LANG_EN_US] = "Shutting down...";
    texts[TXT_RESTART_MSG + LANG_EN_US] = "Restarting...";
    texts[TXT_SLEEP_MSG + LANG_EN_US] = "Sleeping...";
    texts[TXT_FW_MSG + LANG_EN_US] = "Entering firmware (BIOS/UEFI)...";
    texts[TXT_REC_MSG + LANG_EN_US] = "Entering recovery (WinRE)...";
    texts[TXT_RETURN_VALUE + LANG_EN_US] = "Return value: ";
    texts[TXT_ARGS_HEADER + LANG_EN_US] = "Startup parameters (%d total):";
    texts[TXT_ARGS_ITEM + LANG_EN_US] = "  args[%d] = %s";
    texts[TXT_NO_ARGS + LANG_EN_US] = "  (none)";
    texts[TXT_CRASH_FAIL + LANG_EN_US] = "Crash failed...";
    texts[TXT_SWITCHING_CP + LANG_EN_US] = "Switching Code Page......\nPlease wait......\n";
    texts[TXT_LANG_CHANGED + LANG_EN_US] = "Language changed to %s";
    texts[TXT_LANG_UNSUPPORTED + LANG_EN_US] = "Unsupported language.";
    texts[TXT_LANG_PROMPT + LANG_EN_US] = "Current: %s\nAvailable: zh-CN, en-US\nEnter code: ";
    texts[TXT_ALARM_LAUNCH + LANG_EN_US] = "Launching alarm program";
    texts[TXT_TIME_NOW + LANG_EN_US] = "Current time: ";
    texts[TXT_DATE_NOW + LANG_EN_US] = "Current date: ";
    texts[TXT_LOOPTIME_EXIT + LANG_EN_US] = "Press any key to exit loop...";
    texts[TXT_JIYUKILL_LAUNCH + LANG_EN_US] = "Launching JiYuKiller.exe to terminate StudentMain.exe...";
    texts[TXT_TIMER_LAUNCH + LANG_EN_US] = "Launching timer program";
    texts[TXT_TIMER_NOT_FOUND + LANG_EN_US] = "Error: Timer-*.exe not found.";
}

// ============ 工具函数 ============
static string Upper(string s) {
    for (int i = 0; i < s.size(); i++) s[i] = toupper(s[i]);
    return s;
}

static vector<string> SplitStrBySpace(const string& x) {
    vector<string> result;
    string temp;
    for (char c : x) {
        if (isspace(static_cast<unsigned char>(c))) {
            if (!temp.empty()) { result.push_back(temp); temp.clear(); }
        }
        else temp += c;
    }
    if (!temp.empty()) result.push_back(temp);
    return result;
}

static bool CheckFileIsVaild(string FilePath) {
    if (FilePath.empty()) {
        cout << GetText(TXT_FILE_PATH_EMPTY) << endl;
        return false;
    }
    ifstream file(FilePath);
    if (!file.is_open()) {
        cout << FilePath << " " << GetText(TXT_FILE_OPEN_FAIL) << endl;
        return false;
    }
    file.close();
    return true;
}

static void RunProgram(const string& programName) {
    if (programName.empty()) { cout << GetText(TXT_PROGRAM_NAME_EMPTY) << endl; return; }
    if (programName.find_first_of("\\/:*?\"<>|") != string::npos) { cout << GetText(TXT_PROGRAM_NAME_ILLEGAL) << endl; return; }
    if (programName.find_first_of(" \t") != string::npos) { cout << GetText(TXT_PROGRAM_NAME_SPACE) << endl; return; }
    if (programName == "TOOLBOX_ONLY_CMD_LAUNCH") {
        if (CheckFileIsVaild("C:\\Windows\\System32\\cmd.exe")) {
            int rv = system("C:\\Windows\\System32\\cmd.exe");
            cout << GetText(TXT_RETURN_VALUE) << rv << endl;
        }
    }
    if (CheckFileIsVaild(programName)) {
        int rv = system(programName.c_str());
        cout << GetText(TXT_RETURN_VALUE) << rv << endl;
    }
}

// ============ 主逻辑 ============
static void ToolBox(int argc, char** argv, bool debuginput = false) {
    cout << GetText(TXT_SWITCHING_CP);
    bool debug = debuginput;
    vector<string> args;
    for (int i = 0; i < argc; ++i) args.push_back(argv[i]);
    system("chcp 936");
    system("cls");
    cout << GetText(TXT_WELCOME) << "\n";
    if (debug) {
        cout << "[DEBUG] Debug mode initialized from command line (--debug)" << endl;
    }

    while (true) {
        cout << GetText(TXT_PROMPT);
        string command;
        getline(cin, command);

        // RAII 计时器：离开本轮作用域时打印耗时
        ScopeTimer __timer(debug);

        if (debug) {
            cout << "[DEBUG] Raw input  : \"" << command << "\"" << endl;
            cout << "[DEBUG] Length     : " << command.size() << endl;
            auto tokens = SplitStrBySpace(command);
            cout << "[DEBUG] Tokens (" << tokens.size() << "):";
            for (size_t i = 0; i < tokens.size(); i++) {
                cout << " [" << i << "]=\"" << tokens[i] << "\"";
            }
            cout << endl;
        }

        command = Upper(command);

        if (debug) {
            cout << "[DEBUG] Uppercased : \"" << command << "\"" << endl;
        }

        if (command == "HELP") {
            cout << GetText(TXT_HELP_HEADER) << "\n";
            cout << GetText(TXT_HELP_UPDLOG) << "\n";
            cout << GetText(TXT_HELP_VER) << "\n";
            cout << GetText(TXT_HELP_HELP) << "\n";
            cout << GetText(TXT_HELP_EXIT) << "\n";
            cout << GetText(TXT_HELP_EXITF) << "\n";
            cout << GetText(TXT_HELP_EXITF_DESC) << "\n";
            cout << GetText(TXT_HELP_ABORTF) << "\n";
            cout << GetText(TXT_HELP_CRASH) << "\n";
            cout << GetText(TXT_HELP_BADCRASH) << "\n";
            cout << GetText(TXT_HELP_CMD) << "\n";
            cout << GetText(TXT_HELP_MKE) << "\n";
            cout << GetText(TXT_HELP_RF) << "\n";
            cout << GetText(TXT_HELP_SHUTDOWN) << "\n";
            cout << GetText(TXT_HELP_RESTART) << "\n";
            cout << GetText(TXT_HELP_EMERGENCY) << "\n";
            cout << GetText(TXT_HELP_SLEEP) << "\n";
            cout << GetText(TXT_HELP_FW) << "\n";
            cout << GetText(TXT_HELP_REC) << "\n";
            cout << GetText(TXT_HELP_RUN) << "\n";
            cout << GetText(TXT_HELP_CLS) << "\n";
            cout << GetText(TXT_HELP_LANG) << "\n";
            cout << GetText(TXT_HELP_TIME) << "\n";
            cout << GetText(TXT_HELP_LOOPTIME) << "\n";
            cout << GetText(TXT_HELP_DATE) << "\n";
            cout << GetText(TXT_HELP_ALARM) << "\n";
            cout << GetText(TXT_HELP_JIYUKILL) << "\n";
            cout << GetText(TXT_HELP_TIMER) << "\n";
            cout << GetText(TXT_HELP_MEMORY) << "\n";
            cout << GetText(TXT_HELP_REPAIR) << "\n";
            cout << GetText(TXT_HELP_PROGRAM_RUN_HEADER) << "\n";
            cout << GetText(TXT_HELP_PROG1) << "\n";
            cout << GetText(TXT_HELP_PROG2) << "\n";
            cout << GetText(TXT_HELP_PROG3) << "\n";
            cout << GetText(TXT_HELP_PROG4) << "\n";
            cout << GetText(TXT_HELP_PROG5) << "\n";
            cout << GetText(TXT_HELP_PROGB1) << "\n";
            cout << GetText(TXT_HELP_PROGB2) << "\n";
            cout << GetText(TXT_HELP_DEBUG_HEADER) << "\n";
            cout << GetText(TXT_HELP_DEBUG) << "\n";
            cout << GetText(TXT_HELP_UNDEBUG) << "\n";
        }
        else if (command == "VER") cout << GetText(TXT_VER_INFO) << "\n";
        else if (command == "EXIT") return;
        else if (command == "EXITF") exit(0);
        else if (command == "ABORTF") abort();
        else if (command == "CRASH") {
            int* nullptr_ = NULL;
            cout << *nullptr_ << endl;
            cout << GetText(TXT_CRASH_FAIL) << "\n";
        }
        else if (command == "BADCRASH") {
            cout << GetText(TXT_BADCRASH_MSG) << endl;
            BadCrashRecursive();
            cout << GetText(TXT_CRASH_FAIL) << "\n";
        }
        else if (command == "EMERGENCY") {
            cout << GetText(TXT_EMERGENCY_MSG) << endl;
            system("shutdown /s /f /t 0");
        }
        else if (command == "MEMORY") {
            MEMORYSTATUSEX memInfo;
            memInfo.dwLength = sizeof(MEMORYSTATUSEX);
            if (GlobalMemoryStatusEx(&memInfo)) {
                DWORDLONG totalMB = memInfo.ullTotalPhys / (1024 * 1024);
                DWORDLONG availMB = memInfo.ullAvailPhys / (1024 * 1024);
                DWORDLONG usedMB = totalMB - availMB;
                cout << GetText(TXT_MEMORY_HEADER) << endl;
                cout << "Total: " << totalMB << " MB" << endl;
                cout << "Used:  " << usedMB << " MB" << endl;
                cout << "Free:  " << availMB << " MB" << endl;
                cout << "Load:  " << memInfo.dwMemoryLoad << "%" << endl;
            }
            else {
                cout << GetText(TXT_MEMORY_ERROR) << endl;
            }
        }
        else if (command == "REPAIR") {
            cout << GetText(TXT_REPAIR_MSG) << endl;
            system("sfc /scannow");
        }
        else if (command == "CMD") {
            if (CheckFileIsVaild("C:\\Windows\\System32\\cmd.exe")) RunProgram("cmd");
        }
        else if (command == "PROG1") { if (CheckFileIsVaild("PROGRAM1.EXE")) RunProgram("PROGRAM1.EXE"); }
        else if (command == "PROG2") { if (CheckFileIsVaild("PROGRAM2.EXE")) RunProgram("PROGRAM2.EXE"); }
        else if (command == "PROG3") { if (CheckFileIsVaild("PROGRAM3.EXE")) RunProgram("PROGRAM3.EXE"); }
        else if (command == "PROG4") { if (CheckFileIsVaild("PROGRAM4.EXE")) RunProgram("PROGRAM4.EXE"); }
        else if (command == "PROG5") { if (CheckFileIsVaild("PROGRAM5.EXE")) RunProgram("PROGRAM5.EXE"); }
        else if (command == "PROGB1") { if (CheckFileIsVaild("PROGRAM1.BAT")) RunProgram("PROGRAM1.BAT"); }
        else if (command == "PROGB2") { if (CheckFileIsVaild("PROGRAM2.BAT")) RunProgram("PROGRAM2.BAT"); }

        // ============ 增强版 DEBUG 命令 ============
        else if (command == "DEBUG" || command.rfind("DEBUG ", 0) == 0) {
            auto parts = SplitStrBySpace(command);
            string sub = (parts.size() >= 2) ? parts[1] : "";

            if (sub.empty() || sub == "TRUE" || sub == "ON" || sub == "1" || sub == "YES") {
                debug = true;
                cout << "[DEBUG] Debug mode: ON" << endl;
            }
            else if (sub == "FALSE" || sub == "OFF" || sub == "0" || sub == "NO") {
                debug = false;
                cout << "[DEBUG] Debug mode: OFF" << endl;
            }
            else if (sub == "INFO" || sub == "STATUS") {
                cout << "[DEBUG] ---- Debug Info ----" << endl;
                cout << "[DEBUG] Mode      : " << (debug ? "ON" : "OFF") << endl;
                cout << "[DEBUG] Language  : " << g_currentLang << endl;
                cout << "[DEBUG] Version   : " << GetText(TXT_VER_INFO) << endl;
            }
            else if (sub == "PATH") {
                char cwd[MAX_PATH] = { 0 };
                GetCurrentDirectoryA(MAX_PATH, cwd);
                cout << "[DEBUG] CWD      : " << cwd << endl;
                cout << "[DEBUG] Exe path : " << argv[0] << endl;
            }
            else if (sub == "ARGS") {
                cout << "[DEBUG] argc = " << argc << endl;
                for (int i = 0; i < argc; i++) {
                    cout << "[DEBUG] argv[" << i << "] = " << argv[i] << endl;
                }
            }
            else if (sub == "TIME") {
                time_t now = time(nullptr);
                tm tm_now;
                localtime_s(&tm_now, &now);
                char buf[32];
                strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_now);
                cout << "[DEBUG] Time : " << buf << endl;
            }
            else if (sub == "ENV") {
                const char* keys[] = { "COMPUTERNAME", "USERNAME", "OS", "TEMP", "PATH" };
                for (auto k : keys) {
                    const char* v = getenv(k);
                    cout << "[DEBUG] " << k << " = " << (v ? v : "(null)") << endl;
                }
            }
            else if (sub == "BREAK") {
                cout << "[DEBUG] Triggering __debugbreak()..." << endl;
                __debugbreak();
            }
            else if (sub == "CRASH") {
                cout << "[DEBUG] Triggering CRASH for debugging..." << endl;
                int* p = nullptr;
                cout << *p << endl;
            }
            else if (sub == "HELP" || sub == "?") {
                cout << "[DEBUG] Subcommands:" << endl;
                cout << "  DEBUG                 开启调试模式" << endl;
                cout << "  DEBUG TRUE / FALSE    开/关" << endl;
                cout << "  DEBUG ON / OFF        开/关" << endl;
                cout << "  DEBUG 1 / 0           开/关" << endl;
                cout << "  DEBUG YES / NO        开/关" << endl;
                cout << "  DEBUG INFO            显示调试状态" << endl;
                cout << "  DEBUG PATH            显示工作目录和 exe 路径" << endl;
                cout << "  DEBUG ARGS            显示启动参数" << endl;
                cout << "  DEBUG TIME            显示当前时间" << endl;
                cout << "  DEBUG ENV             显示部分环境变量" << endl;
                cout << "  DEBUG BREAK           触发断点（附 VS 调试器时中断）" << endl;
                cout << "  DEBUG CRASH           触发崩溃（用于测试）" << endl;
            }
            else {
                cout << "[DEBUG] Unknown subcommand: " << sub << endl;
                cout << "[DEBUG] Use 'DEBUG HELP' for help" << endl;
            }
        }
        else if (command == "UNDEBUG") debug = false;

        else if (command == "RF") {
            printf(GetText(TXT_ARGS_HEADER).c_str(), (int)args.size());
            cout << "\n";
            if (args.empty()) cout << GetText(TXT_NO_ARGS) << "\n";
            else for (size_t i = 0; i < args.size(); ++i) {
                printf(GetText(TXT_ARGS_ITEM).c_str(), (int)i, args[i].c_str());
                cout << "\n";
            }
        }
        else if (command == "UPDLOG") {
            cout << GetText(TXT_UPDLOG_TITLE) << "\n";
            cout << GetText(TXT_UPDLOG_CONTENT) << "\n";
        }
        else if (command == "MKE") cout << GetText(TXT_MKE_CONTENT) << "\n";
        else if (command == "SHUTDOWN") { cout << GetText(TXT_SHUTDOWN_MSG) << "\n"; system("shutdown /s /t 1"); }
        else if (command == "RESTART") { cout << GetText(TXT_RESTART_MSG) << "\n"; system("shutdown /r /t 1"); }
        else if (command == "SLEEP") { cout << GetText(TXT_SLEEP_MSG) << "\n"; system("rundll32.exe powrprof.dll,SetSuspendState 0,1,0"); }
        else if (command == "FW") { cout << GetText(TXT_FW_MSG) << "\n"; system("shutdown /r /fw /t 1"); }
        else if (command == "REC") { cout << GetText(TXT_REC_MSG) << "\n"; system("shutdown /r /o /t 1"); }
        else if (command == "CLS") system("cls");
        else if (command == "RUN") {
            cout << GetText(TXT_RUN_PROMPT);
            string programName;
            getline(cin, programName);
            if (CheckFileIsVaild(programName)) RunProgram(programName);
        }
        else if (command == "LANG") {
            printf(GetText(TXT_LANG_PROMPT).c_str(), g_currentLang.c_str());
            string newLang;
            getline(cin, newLang);
            if (newLang == "zh-CN" || newLang == "en-US") {
                g_currentLang = newLang;
                printf(GetText(TXT_LANG_CHANGED).c_str(), newLang.c_str());
                cout << endl;
            }
            else cout << GetText(TXT_LANG_UNSUPPORTED) << endl;
        }
        else if (command == "TIME") {
            time_t now = time(nullptr);
            tm tm_now; localtime_s(&tm_now, &now);
            char buf[20]; strftime(buf, sizeof(buf), "%H:%M:%S", &tm_now);
            cout << GetText(TXT_TIME_NOW) << buf << endl;
        }
        else if (command == "DATE") {
            time_t now = time(nullptr);
            tm tm_now; localtime_s(&tm_now, &now);
            char buf[20]; strftime(buf, sizeof(buf), "%Y-%m-%d", &tm_now);
            cout << GetText(TXT_DATE_NOW) << buf << endl;
        }
        else if (command == "LOOPTIME") {
            cout << GetText(TXT_LOOPTIME_EXIT) << endl;
            while (!_kbhit()) {
                time_t now = time(nullptr);
                tm tm_now; localtime_s(&tm_now, &now);
                char buf[20]; strftime(buf, sizeof(buf), "%H:%M:%S", &tm_now);
                cout << "\r" << GetText(TXT_TIME_NOW) << buf << flush;
                this_thread::sleep_for(chrono::milliseconds(500));
            }
            _getch(); cout << endl;
        }
        else if (command == "ALARM") {
            namespace fs = std::filesystem;
            string exe; bool found = false;
            for (const auto& entry : fs::directory_iterator(".")) {
                if (entry.is_regular_file()) {
                    string fn = entry.path().filename().string();
                    if (fn.size() > 13 && fn.compare(0, 11, "AlarmClock-") == 0 && fn.compare(fn.size() - 4, 4, ".exe") == 0) {
                        exe = fn; found = true; break;
                    }
                }
            }
            if (found) {
                cout << GetText(TXT_ALARM_LAUNCH) << " (" << exe << ")" << endl;
                string cmd = "start " + exe;
                system(cmd.c_str());
            }
            else {
                cout << "错误：找不到 AlarmClock-*.exe 文件。" << endl;
            }
        }
        else if (command == "JIYUKILL" || command == "JYKILL") {
            if (CheckFileIsVaild("JiYuKiller.exe")) {
                cout << GetText(TXT_JIYUKILL_LAUNCH) << endl;
                system("JiYuKiller.exe StudentMain.exe");
            }
        }
        else if (command == "TIMER") {
            namespace fs = std::filesystem;
            string exe; bool found = false;
            for (const auto& entry : fs::directory_iterator(".")) {
                if (entry.is_regular_file()) {
                    string fn = entry.path().filename().string();
                    if (fn.size() > 9 && fn.compare(0, 6, "Timer-") == 0 && fn.compare(fn.size() - 4, 4, ".exe") == 0) {
                        exe = fn; found = true; break;
                    }
                }
            }
            if (found) {
                cout << GetText(TXT_TIMER_LAUNCH) << " (" << exe << ")" << endl;
                string cmd = "start " + exe;
                system(cmd.c_str());
            }
            else {
                cout << GetText(TXT_TIMER_NOT_FOUND) << endl;
            }
        }
        else if (command == "") { /* 空指令 */ }
        else {
            auto parts = SplitStrBySpace(command);
            if (parts.size() > 1 && parts[0] == "EXITF") {
                string exitCodeStr = parts[1];
                try {
                    int exitCode = stoi(exitCodeStr);
                    exit(exitCode);
                }
                catch (const invalid_argument&) {
                    cout << GetText(TXT_EXITF_INVALID) << "\n";
                }
                catch (const out_of_range&) {
                    cout << GetText(TXT_EXITF_OUTRANGE) << "\n";
                }
                catch (const exception& e) {
                    cout << GetText(TXT_EXITF_ERROR) << e.what() << endl;
                }
                catch (...) {
                    cout << GetText(TXT_EXITF_UNKNOWN) << "\n";
                }
            }
            else {
                cout << GetText(TXT_CMD_UNKNOWN) << "\n";
            }
        }
    }
}

int main(int argc, char** argv) {
    InitTexts();
    bool debug = false;
    cout << GetText(TXT_LOADING) << "\n";
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "--debug") { debug = true; break; }
    }
    ToolBox(argc, argv, debug);
    return 0;
}