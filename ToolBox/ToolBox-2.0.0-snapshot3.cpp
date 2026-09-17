#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <conio.h>      // _kbhit
#include <thread>
#include <chrono>
#include <filesystem>   // 用于查找文件
using namespace std;

// ============ 多语言支持相关定义 ============
enum TextID {
    TXT_LOADING,
    TXT_WELCOME,
    TXT_PROMPT,
    TXT_HELP_HEADER,
    TXT_HELP_UPDLOG,
    TXT_HELP_VER,
    TXT_HELP_HELP,
    TXT_HELP_EXIT,
    TXT_HELP_EXITF,
    TXT_HELP_EXITF_DESC,
    TXT_HELP_ABORTF,
    TXT_HELP_CRASH,
    TXT_HELP_CMD,
    TXT_HELP_MKE,
    TXT_HELP_RF,
    TXT_HELP_SHUTDOWN,
    TXT_HELP_RESTART,
    TXT_HELP_SLEEP,
    TXT_HELP_FW,
    TXT_HELP_REC,
    TXT_HELP_RUN,
    TXT_HELP_CLS,
    TXT_HELP_PROG1,
    TXT_HELP_PROG2,
    TXT_HELP_PROG3,
    TXT_HELP_PROG4,
    TXT_HELP_PROG5,
    TXT_HELP_PROGB1,
    TXT_HELP_PROGB2,
    TXT_HELP_DEBUG,
    TXT_HELP_UNDEBUG,
    TXT_HELP_PROGRAM_RUN_HEADER,
    TXT_HELP_DEBUG_HEADER,
    TXT_VER_INFO,
    TXT_UPDLOG_TITLE,
    TXT_UPDLOG_CONTENT,
    TXT_MKE_CONTENT,
    TXT_CMD_UNKNOWN,
    TXT_EXITF_INVALID,
    TXT_EXITF_OUTRANGE,
    TXT_EXITF_ERROR,
    TXT_EXITF_UNKNOWN,
    TXT_FILE_PATH_EMPTY,
    TXT_FILE_OPEN_FAIL,
    TXT_PROGRAM_NAME_EMPTY,
    TXT_PROGRAM_NAME_ILLEGAL,
    TXT_PROGRAM_NAME_SPACE,
    TXT_RUN_PROMPT,
    TXT_SHUTDOWN_MSG,
    TXT_RESTART_MSG,
    TXT_SLEEP_MSG,
    TXT_FW_MSG,
    TXT_REC_MSG,
    TXT_RETURN_VALUE,
    TXT_ARGS_HEADER,
    TXT_ARGS_ITEM,
    TXT_NO_ARGS,
    TXT_CRASH_FAIL,
    TXT_SWITCHING_CP,
    TXT_LANG_CHANGED,
    TXT_LANG_UNSUPPORTED,
    TXT_LANG_PROMPT,
    TXT_HELP_LANG,
    // ----- 新增 -----
    TXT_HELP_TIME,
    TXT_HELP_LOOPTIME,
    TXT_HELP_DATE,
    TXT_HELP_ALARM,
    TXT_ALARM_LAUNCH,
    TXT_TIME_NOW,
    TXT_DATE_NOW,
    TXT_LOOPTIME_EXIT,
    TXT_COUNT
};

const int LANG_ZH_CN = 0;
const int LANG_EN_US = 10000;

string texts[500000] = {};
string g_currentLang = "zh-CN";

int GetLangOffset() {
    if (g_currentLang == "en-US") return LANG_EN_US;
    return LANG_ZH_CN;
}

string GetText(TextID id) {
    return texts[id + GetLangOffset()];
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
    texts[TXT_HELP_DEBUG + LANG_ZH_CN] = "DEBUG    打开调试模式（没有什么功能）";
    texts[TXT_HELP_UNDEBUG + LANG_ZH_CN] = "UNDEBUG  关闭调试模式";
    // ----- 新增帮助 -----
    texts[TXT_HELP_TIME + LANG_ZH_CN] = "TIME     查看当前时间";
    texts[TXT_HELP_LOOPTIME + LANG_ZH_CN] = "LOOPTIME 循环显示时间（按任意键退出）";
    texts[TXT_HELP_DATE + LANG_ZH_CN] = "DATE     查看当前日期";
    texts[TXT_HELP_ALARM + LANG_ZH_CN] = "ALARM    打开闹钟设置（独立 GUI）";
    // -----
    texts[TXT_VER_INFO + LANG_ZH_CN] = "Tool-Box 2.0.0-snapshot3";
    texts[TXT_UPDLOG_TITLE + LANG_ZH_CN] = "更新日志：";
    texts[TXT_UPDLOG_CONTENT + LANG_ZH_CN] = R"(
2026-08-06
Dev 阶段: 已经跳过
Alpha 阶段: 已经跳过
Beta 阶段: 已经跳过
Pre-Release 阶段: 已经丢失
1.0.0: 正式版发布
1.1.0: 增加 RF 指令和一些优化，并且平台移动到 VS 2026
1.2.0: 增加了 UPDLOG 指令，更改了 NULLPTR 崩溃的 NULLPTR 指针的变量名
1.2.1: 修改了 PROG1-PROG5 和 PROGB1-PROGB2 和 CMD 的执行方式：增加了文件有效性检查
1.2.2: 由于 VS 2026 无法使用 DBZ 崩溃，所以把 DBZ 移除了
1.2.3: 计划实现输入参数的功能，但是导致了亿个严重的 BUG 和 CE
1.2.4: 回退了 1.1.3 的更改。
1.2.5: 在之前几个版本实现了代码页功能，但是那个代码页切换的成功提示太烦人了，所以就用 CLS 清除了 :)
1.3.0: 再次尝试实现输入参数的功能，这次成功了
1.4.0: 加入了 MKE 指令
1.5.0: 使用 getline() 来输入数据，解决了 1 个问题，但是，参数功能却无法使用了……[计划在1.4回归]
1.5.1: 修复了空指令是未知指令的问题
1.6.0: 回归 Pre-Release 1.3 的 CMD 指令，并且改名为 RUN 指令
1.7.0: 加入了 SHUTDOWN 和 RESTART 指令
1.8.0: 增加了 CLS 指令，并且把 HELP 里的字符串调成了 50 长度
2026-08-07
1.9.0: 增加了 SLEEP、FW、REC 指令
2026-08-08
1.9.1: 更改了 UPDLOG 指令的输出格式 和 MKE 指令的输出内容
2026-08-09
1.10.0: 增加了 UPDF 指令，并且更改了版本号格式
2026-08-10
2.0.0-beta: 多语言加入工具箱，HELP 的输出改变，由 DeepSeek-R1 制作
2.0.0-sp1: Nothings
2026-08-30
2.0.0-snapshot2: 增加 TIME, LOOPTIME, DATE, ALARM 指令
2026-08-31
2.0.0-snapshot3: ALARM 指令自动查找 AlarmClock-*.exe，更新闹钟集成
)";
    texts[TXT_MKE_CONTENT + LANG_ZH_CN] = R"(
作者：
YuanMT（主作者 | Bilibili: https://space.bilibili.com/3546939977107567）
DeepSeek v4&r1（协助 | Address: https://www.deepseek.com）
Visual Studio AI（协助）
TT一TT（网站 | https://tt-web.gt.tc）
如果你想要在此处看到你的名字，请至少协助 1 个版本的开发（通过 https://space.bilibili.com/3546939977107567 联系，并且关注我）
)";
    texts[TXT_CMD_UNKNOWN + LANG_ZH_CN] = "无效的指令！";
    texts[TXT_EXITF_INVALID + LANG_ZH_CN] = "无效的退出代码！请确保输入的是一个整数。";
    texts[TXT_EXITF_OUTRANGE + LANG_ZH_CN] = "退出代码超出范围！请确保输入的是一个有效的整数。";
    texts[TXT_EXITF_ERROR + LANG_ZH_CN] = "发生错误：";
    texts[TXT_EXITF_UNKNOWN + LANG_ZH_CN] = "发生未知错误！";
    texts[TXT_FILE_PATH_EMPTY + LANG_ZH_CN] = "文件路径为空！请检查输入。";
    texts[TXT_FILE_OPEN_FAIL + LANG_ZH_CN] = "文件打开失败！请检查文件是否存在，或者文件名称是否正确。";
    texts[TXT_PROGRAM_NAME_EMPTY + LANG_ZH_CN] = "程序名称为空！请检查输入。";
    texts[TXT_PROGRAM_NAME_ILLEGAL + LANG_ZH_CN] = "程序名称包含非法字符！请检查输入。";
    texts[TXT_PROGRAM_NAME_SPACE + LANG_ZH_CN] = "程序名称包含空格或制表符！请检查输入。";
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
    // 新增中文
    texts[TXT_ALARM_LAUNCH + LANG_ZH_CN] = "正在启动闹钟程序";
    texts[TXT_TIME_NOW + LANG_ZH_CN] = "当前时间：";
    texts[TXT_DATE_NOW + LANG_ZH_CN] = "当前日期：";
    texts[TXT_LOOPTIME_EXIT + LANG_ZH_CN] = "按任意键退出循环……";

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
    texts[TXT_HELP_DEBUG + LANG_EN_US] = "DEBUG    Enable debug mode (not much)";
    texts[TXT_HELP_UNDEBUG + LANG_EN_US] = "UNDEBUG  Disable debug mode";
    // ----- 新增帮助 -----
    texts[TXT_HELP_TIME + LANG_EN_US] = "TIME     Show current time";
    texts[TXT_HELP_LOOPTIME + LANG_EN_US] = "LOOPTIME Loop show time (press any key to exit)";
    texts[TXT_HELP_DATE + LANG_EN_US] = "DATE     Show current date";
    texts[TXT_HELP_ALARM + LANG_EN_US] = "ALARM    Open alarm GUI (standalone)";
    // -----
    texts[TXT_VER_INFO + LANG_EN_US] = "Tool-Box 2.0.0-snapshot3";
    texts[TXT_UPDLOG_TITLE + LANG_EN_US] = "Update log:";
    texts[TXT_UPDLOG_CONTENT + LANG_EN_US] = R"(
2026-08-06
Dev phase: skipped
Alpha phase: skipped
Beta phase: skipped
Pre-Release phase: lost
1.0.0: Official release
1.1.0: Added RF command and some optimizations, moved to VS 2026
1.2.0: Added UPDLOG command, changed nullptr variable name
1.2.1: Modified PROG1-PROG5, PROGB1-PROGB2 and CMD execution: added file validation
1.2.2: Removed DBZ crash as VS 2026 can't use it
1.2.3: Planned input parameter feature, but caused many bugs and crashes
1.2.4: Reverted 1.1.3 changes.
1.2.5: Code page switching implemented, but success prompt was annoying so cleared with CLS :)
1.3.0: Attempted input parameter feature again, succeeded
1.4.0: Added MKE command
1.5.0: Used getline() for input, fixed one issue but broke parameter feature... [planned return in 1.4]
1.5.1: Fixed empty command treated as unknown
1.6.0: Reintroduced Pre-Release 1.3 CMD command, renamed to RUN
1.7.0: Added SHUTDOWN and RESTART commands
1.8.0: Added CLS command, adjusted HELP string length to 50
2026-08-07
1.9.0: Added SLEEP, FW, REC commands
2026-08-08
1.9.1: Changed UPDLOG output format and MKE output content
2026-08-09
1.10.0: Added UPDF command, changed version format
2026-08-10
2.0.0: Language is added! HELP's output is changed! By DeepSeek-R1
2.0.0-sp1: Nothings
2026-08-30
2.0.0-snapshot2: Added TIME, LOOPTIME, DATE, ALARM commands
2026-08-31
2.0.0-snapshot3: ALARM now auto-finds AlarmClock-*.exe, integrated alarm update
)";
    texts[TXT_MKE_CONTENT + LANG_EN_US] = R"(
Author:
YuanMT (Main author | Bilibili: https://space.bilibili.com/3546939977107567)
DeepSeek v4&r1 (Assistance | https://www.deepseek.com)
Visual Studio AI (Assistance)
TT一TT (Website | https://tt-web.gt.tc)
If you want your name here, please help develop at least 1 version (contact via Bilibili)
)";
    texts[TXT_CMD_UNKNOWN + LANG_EN_US] = "Unknown command!";
    texts[TXT_EXITF_INVALID + LANG_EN_US] = "Invalid exit code! Please enter an integer.";
    texts[TXT_EXITF_OUTRANGE + LANG_EN_US] = "Exit code out of range! Please enter a valid integer.";
    texts[TXT_EXITF_ERROR + LANG_EN_US] = "Error: ";
    texts[TXT_EXITF_UNKNOWN + LANG_EN_US] = "Unknown error!";
    texts[TXT_FILE_PATH_EMPTY + LANG_EN_US] = "File path is empty! Check input.";
    texts[TXT_FILE_OPEN_FAIL + LANG_EN_US] = "File open failed! Please check if the file exists or the name is correct.";
    texts[TXT_PROGRAM_NAME_EMPTY + LANG_EN_US] = "Program name is empty! Check input.";
    texts[TXT_PROGRAM_NAME_ILLEGAL + LANG_EN_US] = "Program name contains illegal characters! Check input.";
    texts[TXT_PROGRAM_NAME_SPACE + LANG_EN_US] = "Program name contains spaces or tabs! Check input.";
    texts[TXT_RUN_PROMPT + LANG_EN_US] = "Enter the program name (with extension): ";
    texts[TXT_SHUTDOWN_MSG + LANG_EN_US] = "Shutting down...";
    texts[TXT_RESTART_MSG + LANG_EN_US] = "Restarting...";
    texts[TXT_SLEEP_MSG + LANG_EN_US] = "Sleeping...";
    texts[TXT_FW_MSG + LANG_EN_US] = "Entering firmware (BIOS/UEFI)...";
    texts[TXT_REC_MSG + LANG_EN_US] = "Entering recovery environment (WinRE)...";
    texts[TXT_RETURN_VALUE + LANG_EN_US] = "Return value: ";
    texts[TXT_ARGS_HEADER + LANG_EN_US] = "Startup parameters (%d total):";
    texts[TXT_ARGS_ITEM + LANG_EN_US] = "  args[%d] = %s";
    texts[TXT_NO_ARGS + LANG_EN_US] = "  (none)";
    texts[TXT_CRASH_FAIL + LANG_EN_US] = "Crash failed...";
    texts[TXT_SWITCHING_CP + LANG_EN_US] = "Switching Code Page......\nPlease wait......\n";
    texts[TXT_LANG_CHANGED + LANG_EN_US] = "Language changed to %s";
    texts[TXT_LANG_UNSUPPORTED + LANG_EN_US] = "Unsupported language.";
    texts[TXT_LANG_PROMPT + LANG_EN_US] = "Current language: %s\nAvailable: zh-CN, en-US\nEnter language code: ";
    // 新增英文
    texts[TXT_ALARM_LAUNCH + LANG_EN_US] = "Launching alarm program";
    texts[TXT_TIME_NOW + LANG_EN_US] = "Current time: ";
    texts[TXT_DATE_NOW + LANG_EN_US] = "Current date: ";
    texts[TXT_LOOPTIME_EXIT + LANG_EN_US] = "Press any key to exit loop...";
}

// ============ 原有工具函数 ============
static string Upper(string s) {
    for (int i = 0; i < s.size(); i++) {
        s[i] = toupper(s[i]);
    }
    return s;
}

static vector<string> SplitStrBySpace(const string& x) {
    vector<string> result;
    string temp;
    for (char c : x) {
        if (isspace(static_cast<unsigned char>(c))) {
            if (!temp.empty()) {
                result.push_back(temp);
                temp.clear();
            }
        }
        else {
            temp += c;
        }
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
    if (programName.empty()) {
        cout << GetText(TXT_PROGRAM_NAME_EMPTY) << endl;
        return;
    }
    if (programName.find_first_of("\\/:*?\"<>|") != string::npos) {
        cout << GetText(TXT_PROGRAM_NAME_ILLEGAL) << endl;
        return;
    }
    if (programName.find_first_of(" \t") != string::npos) {
        cout << GetText(TXT_PROGRAM_NAME_SPACE) << endl;
        return;
    }
    if (programName == "TOOLBOX_ONLY_CMD_LAUNCH") {
        if (CheckFileIsVaild("C:\\Windows\\System32\\cmd.exe")) {
            int returnValue = system("C:\\Windows\\System32\\cmd.exe");
            cout << GetText(TXT_RETURN_VALUE) << returnValue << endl;
        }
    }
    if (CheckFileIsVaild(programName)) {
        int returnValue = system(programName.c_str());
        cout << GetText(TXT_RETURN_VALUE) << returnValue << endl;
    }
}

static void ToolBox(int argc, char** argv, bool debuginput = false) {
    cout << GetText(TXT_SWITCHING_CP);
    bool debug = debuginput;
    vector<string> args;
    for (int i = 0; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    system("chcp 936");
    system("cls");
    cout << GetText(TXT_WELCOME) << "\n";
    while (true) {
        cout << GetText(TXT_PROMPT);
        string command;
        getline(cin, command);

        if (debug) cout << "[DEBUG] COMMAND INPUTED: " << command << endl;
        command = Upper(command);

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
            cout << GetText(TXT_HELP_CMD) << "\n";
            cout << GetText(TXT_HELP_MKE) << "\n";
            cout << GetText(TXT_HELP_RF) << "\n";
            cout << GetText(TXT_HELP_SHUTDOWN) << "\n";
            cout << GetText(TXT_HELP_RESTART) << "\n";
            cout << GetText(TXT_HELP_SLEEP) << "\n";
            cout << GetText(TXT_HELP_FW) << "\n";
            cout << GetText(TXT_HELP_REC) << "\n";
            cout << GetText(TXT_HELP_RUN) << "\n";
            cout << GetText(TXT_HELP_CLS) << "\n";
            cout << GetText(TXT_HELP_LANG) << "\n";
            // 新增
            cout << GetText(TXT_HELP_TIME) << "\n";
            cout << GetText(TXT_HELP_LOOPTIME) << "\n";
            cout << GetText(TXT_HELP_DATE) << "\n";
            cout << GetText(TXT_HELP_ALARM) << "\n";
            // 原有
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
        else if (command == "VER") {
            cout << GetText(TXT_VER_INFO) << "\n";
        }
        else if (command == "EXIT") {
            return;
        }
        else if (command == "EXITF") {
            exit(0);
        }
        else if (command == "ABORTF") {
            abort();
        }
        else if (command == "CRASH") {
            int* nullptr_ = NULL;
            cout << *nullptr_ << endl;
            cout << GetText(TXT_CRASH_FAIL) << "\n";
        }
        else if (command == "CMD") {
            if (CheckFileIsVaild("C:\\Windows\\System32\\cmd.exe")) {
                RunProgram("cmd");
            }
        }
        else if (command == "PROG1") {
            if (CheckFileIsVaild("PROGRAM1.EXE")) RunProgram("PROGRAM1.EXE");
        }
        else if (command == "PROG2") {
            if (CheckFileIsVaild("PROGRAM2.EXE")) RunProgram("PROGRAM2.EXE");
        }
        else if (command == "PROG3") {
            if (CheckFileIsVaild("PROGRAM3.EXE")) RunProgram("PROGRAM3.EXE");
        }
        else if (command == "PROG4") {
            if (CheckFileIsVaild("PROGRAM4.EXE")) RunProgram("PROGRAM4.EXE");
        }
        else if (command == "PROG5") {
            if (CheckFileIsVaild("PROGRAM5.EXE")) RunProgram("PROGRAM5.EXE");
        }
        else if (command == "PROGB1") {
            if (CheckFileIsVaild("PROGRAM1.BAT")) RunProgram("PROGRAM1.BAT");
        }
        else if (command == "PROGB2") {
            if (CheckFileIsVaild("PROGRAM2.BAT")) RunProgram("PROGRAM2.BAT");
        }
        else if (command == "DEBUG") {
            debug = true;
        }
        else if (command == "UNDEBUG") {
            debug = false;
        }
        else if (command == "RF") {
            printf(GetText(TXT_ARGS_HEADER).c_str(), (int)args.size());
            cout << "\n";
            if (args.empty()) {
                cout << GetText(TXT_NO_ARGS) << "\n";
            }
            else {
                for (size_t i = 0; i < args.size(); ++i) {
                    printf(GetText(TXT_ARGS_ITEM).c_str(), (int)i, args[i].c_str());
                    cout << "\n";
                }
            }
        }
        else if (command == "UPDLOG") {
            cout << GetText(TXT_UPDLOG_TITLE) << "\n";
            cout << GetText(TXT_UPDLOG_CONTENT) << "\n";
        }
        else if (command == "MKE") {
            cout << GetText(TXT_MKE_CONTENT) << "\n";
        }
        else if (command == "SHUTDOWN") {
            cout << GetText(TXT_SHUTDOWN_MSG) << "\n";
            system("shutdown /s /t 1");
        }
        else if (command == "RESTART") {
            cout << GetText(TXT_RESTART_MSG) << "\n";
            system("shutdown /r /t 1");
        }
        else if (command == "SLEEP") {
            cout << GetText(TXT_SLEEP_MSG) << "\n";
            system("rundll32.exe powrprof.dll,SetSuspendState 0,1,0");
        }
        else if (command == "FW") {
            cout << GetText(TXT_FW_MSG) << "\n";
            system("shutdown /r /fw /t 1");
        }
        else if (command == "REC") {
            cout << GetText(TXT_REC_MSG) << "\n";
            system("shutdown /r /o /t 1");
        }
        else if (command == "CLS") {
            system("cls");
        }
        else if (command == "RUN") {
            cout << GetText(TXT_RUN_PROMPT);
            string programName;
            getline(cin, programName);
            if (CheckFileIsVaild(programName)) {
                RunProgram(programName);
            }
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
            else {
                cout << GetText(TXT_LANG_UNSUPPORTED) << endl;
            }
        }
        // ----- 新增命令 -----
        else if (command == "TIME") {
            time_t now = time(nullptr);
            tm tm_now;
            localtime_s(&tm_now, &now);
            char buf[20];
            strftime(buf, sizeof(buf), "%H:%M:%S", &tm_now);
            cout << GetText(TXT_TIME_NOW) << buf << endl;
        }
        else if (command == "DATE") {
            time_t now = time(nullptr);
            tm tm_now;
            localtime_s(&tm_now, &now);
            char buf[20];
            strftime(buf, sizeof(buf), "%Y-%m-%d", &tm_now);
            cout << GetText(TXT_DATE_NOW) << buf << endl;
        }
        else if (command == "LOOPTIME") {
            cout << GetText(TXT_LOOPTIME_EXIT) << endl;
            while (!_kbhit()) {
                time_t now = time(nullptr);
                tm tm_now;
                localtime_s(&tm_now, &now);
                char buf[20];
                strftime(buf, sizeof(buf), "%H:%M:%S", &tm_now);
                cout << "\r" << GetText(TXT_TIME_NOW) << buf << flush;
                this_thread::sleep_for(chrono::milliseconds(500));
            }
            _getch();
            cout << endl;
        }
        else if (command == "ALARM") {
            // 使用 std::filesystem 查找 AlarmClock-*.exe
            namespace fs = std::filesystem;
            string alarmExe;
            bool found = false;
            for (const auto& entry : fs::directory_iterator(".")) {
                if (entry.is_regular_file()) {
                    string filename = entry.path().filename().string();
                    // 匹配以 "AlarmClock-" 开头，以 ".exe" 结尾
                    if (filename.size() > 13 &&
                        filename.compare(0, 11, "AlarmClock-") == 0 &&
                        filename.compare(filename.size() - 4, 4, ".exe") == 0) {
                        alarmExe = filename;
                        found = true;
                        break;
                    }
                }
            }
            if (found) {
                cout << GetText(TXT_ALARM_LAUNCH) << " (" << alarmExe << ")" << endl;
                string cmd = "start " + alarmExe;
                system(cmd.c_str());
            }
            else {
                cout << "错误：找不到 AlarmClock-*.exe 文件，请确保闹钟程序在工具箱目录下。" << endl;
                cout << "（例如：AlarmClock-1.1.0.exe）" << endl;
            }
        }
        // ----- 结束新增 -----
        else if (command == "") {
            // 空指令无操作
        }
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
        if (string(argv[i]) == "--debug") {
            debug = true;
            break;
        }
    }
    ToolBox(argc, argv, debug);
    return 0;
}