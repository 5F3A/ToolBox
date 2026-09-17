#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <fstream>
string texts[500000] = {};
string langs[200] = { "zh-CN" };
using namespace std;
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
		cout << "文件路径为空！请检查输入。" << endl;
		return false;
	}
	ifstream file(FilePath);
	if (!file.is_open()) {
		cout << "文件 " << FilePath << " 打开失败！请检查文件是否存在，或者文件名称是否正确。" << endl;
		return false;
	}
	file.close();
	return true;
}
static void RunProgram(const string& programName) {
	if (programName.empty()) {
		cout << "程序名称为空！请检查输入。" << endl;
		return;
	}
	if (programName.find_first_of("\\/:*?\"<>|") != string::npos) {
		cout << "程序名称包含非法字符！请检查输入。" << endl;
		return;
	}
	if (programName.find_first_of(" \t") != string::npos) {
		cout << "程序名称包含空格或制表符！请检查输入。" << endl;
		return;
	}
	if (programName == "TOOLBOX_ONLY_CMD_LAUNCH") {
		if (CheckFileIsVaild("C:\\Windows\\System32\\cmd.exe")) {
			int returnValue = system("C:\\Windows\\System32\\cmd.exe");
			cout << "返回值：" << returnValue << endl;
		}
	}
	if (CheckFileIsVaild(programName)) {
		int returnValue = system(programName.c_str());
		cout << "返回值：" << returnValue << endl;
	}
}
static void ToolBox(int argc, char** argv, bool debuginput = false) {
	cout << "Switching Code Page......\nPlease wait......\n";
	bool debug = debuginput;
	vector<string> args;
	for (int i = 0; i < argc; ++i) {
		args.push_back(argv[i]);
	}
	system("chcp 936");
	system("cls");
	cout << "欢迎使用工具箱！输入 help 查看帮助。\n";
	while (true) {
		cout << "请输入指令 > ";
		string command;
		getline(cin, command);

		if (debug) cout << "[DEBUG] COMMAND INPUTED: " << command << endl;
		command = Upper(command);
		if (command == "HELP") {
			cout << "-------------------- HELP-MENU -----------------\n";
			cout << "UPDLOG   更新日志                               \n";
			cout << "VER      查看版本                               \n";
			cout << "HELP     查看帮助                               \n";
			cout << "EXIT     退出程序                               \n";
			cout << "EXITF    使用 exit() 退出程序                   \n";
			cout << "EXITF xx 使用 exit() 退出程序，退出代码为 xx    \n";
			cout << "ABORTF   使用 abort() 退出程序                  \n";
			cout << "CRASH    以 nullptr 崩溃程序                    \n";
			cout << "CMD      启动 CMD                               \n";
			cout << "MKE      显示作者                               \n";
			cout << "RF       查看运行参数                           \n";
			cout << "SHUTDOWN 关机                                   \n";
			cout << "RESTART  重启                                   \n";
			cout << "SLEEP    睡眠                                   \n";
			cout << "FW       转到固件(BIOS/UEFI)                    \n";
			cout << "REC      转到恢复环境(WinRE)                    \n";
			cout << "RUN      执行本工具目录下的程序                 \n";
			cout << "CLS      清除屏幕                               \n";
			cout << "------------------ PROGRAM RUN  ----------------\n";
			cout << "PROG1    执行本工具目录下的 PROGRAM1.EXE        \n";
			cout << "PROG2    执行本工具目录下的 PROGRAM2.EXE        \n";
			cout << "PROG3    执行本工具目录下的 PROGRAM3.EXE        \n";
			cout << "PROG4    执行本工具目录下的 PROGRAM4.EXE        \n";
			cout << "PROG5    执行本工具目录下的 PROGRAM5.EXE        \n";
			cout << "PROGB1   执行本工具目录下的 PROGRAM1.BAT        \n";
			cout << "PROGB2   执行本工具目录下的 PROGRAM2.BAT        \n";
			cout << "---------------------- DEBUG -------------------\n";
			cout << "DEBUG    打开调试模式（没有什么功能）           \n";
			cout << "UNDEBUG  关闭调试模式                           \n";
		}
		else if (command == "VER") {
			cout << "Tool-Box 1.3.1\n";
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
			cout << "崩溃失败……\n";
		}
		else if (command == "CMD") {
			if (CheckFileIsVaild("C:\\Windows\\System32\\cmd.exe")) {
				RunProgram("cmd");
			}
		}
		else if (command == "PROG1") {
			if (CheckFileIsVaild("PROGRAM1.EXE")) {
				RunProgram("PROGRAM1.EXE");
			}
		}
		else if (command == "PROG2") {
			if (CheckFileIsVaild("PROGRAM2.EXE")) {
				RunProgram("PROGRAM2.EXE");
			}
		}
		else if (command == "PROG3") {
			if (CheckFileIsVaild("PROGRAM3.EXE")) {
				RunProgram("PROGRAM3.EXE");
			}
		}
		else if (command == "PROG4") {
			if (CheckFileIsVaild("PROGRAM4.EXE")) {
				RunProgram("PROGRAM4.EXE");
			}
		}
		else if (command == "PROG5") {
			if (CheckFileIsVaild("PROGRAM5.EXE")) {
				RunProgram("PROGRAM5.EXE");
			}
		}
		else if (command == "PROGB1") {
			if (CheckFileIsVaild("PROGRAM1.BAT")) {
				RunProgram("PROGRAM1.BAT");
			}
		}
		else if (command == "PROGB2") {
			if (CheckFileIsVaild("PROGRAM2.BAT")) {
				RunProgram("PROGRAM2.BAT");
			}
		}
		else if (command == "DEBUG") {
			debug = 1;
		}
		else if (command == "UNDEBUG") {
			debug = 0;
		}
		else if (command == "RF") {
			cout << "程序启动参数（共 " << args.size() << " 个）：\n";
			if (args.empty()) {
				cout << "  （无参数）\n";
			}
			else {
				for (size_t i = 0; i < args.size(); ++i) {
					cout << "  args[" << i << "] = " << args[i] << '\n';
				}
			}
		}
		else if (command == "UPDLOG") {
			cout << "更新日志：\n"; 
			cout << "2026-08-06\n";
			cout << "Dev 阶段: 已经跳过\n";
			cout << "Alpha 阶段: 已经跳过\n";
			cout << "Beta 阶段: 已经跳过\n";
			cout << "Pre-Release 阶段: 已经丢失\n";
			cout << "1.0.0: 正式版发布\n";
			cout << "1.1.0: 增加 RF 指令和一些优化，并且平台移动到 VS 2026\n";
			cout << "1.2.0: 增加了 UPDLOG 指令，更改了 NULLPTR 崩溃的 NULLPTR 指针的变量名\n";
			cout << "1.2.1: 修改了 PROG1-PROG5 和 PROGB1-PROGB2 和 CMD 的执行方式：增加了文件有效性检查\n";
			cout << "1.2.2: 由于 VS 2026 无法使用 DBZ 崩溃，所以把 DBZ 移除了\n";
			cout << "1.2.3: 计划实现输入参数的功能，但是导致了亿个严重的 BUG 和 CE\n";
			cout << "1.2.4: 回退了 1.1.3 的更改。\n";
			cout << "1.2.5: 在之前几个版本实现了代码页功能，但是那个代码页切换的成功提示太烦人了，所以就用 CLS 清除了 :)\n";
			cout << "1.3.0: 再次尝试实现输入参数的功能，这次成功了\n";
			cout << "1.4.0: 加入了 MKE 指令\n";
			cout << "1.5.0: 使用 getline() 来输入数据，解决了 1 个问题，但是，参数功能却无法使用了……[计划在1.4回归]\n";
			cout << "1.5.1: 修复了空指令是未知指令的问题\n";
			cout << "1.6.0: 回归 Pre-Release 1.3 的 CMD 指令，并且改名为 RUN 指令\n";
			cout << "1.7.0: 加入了 SHUTDOWN 和 RESTART 指令\n";
			cout << "1.8.0: 增加了 CLS 指令，并且把 HELP 里的字符串调成了 50 长度\n";
			cout << "2026-08-07\n";
			cout << "1.9.0: 增加了 SLEEP、FW、REC 指令\n";
			cout << "2026-08-08\n";
			cout << "1.9.1: 更改了 UPDLOG 指令的输出格式 和 MKE 指令的输出内容\n";
			cout << "2026-08-09\n";
			cout << "1.10.0: 增加了 UPDF 指令，并且更改了版本号格式"; 
			cout << "2026-08-10\n";
			cout << "1.10.1: 删除了 UPDF 指令，并且已经准备在搞 English Lang Support 了。\n";
		}
		else if (command == "MKE") {
			cout << "作者：\nYuanMT（主作者 | Bilibili: https://space.bilibili.com/3546939977107567）\n";
			cout << "DeepSeek v4&r1（协助 | Address: https://www.deepseek.com）\n";
			cout << "Visual Studio AI（协助）\n";
			cout << "如果你想要在此处看到你的名字，请至少协助 1 个版本的开发（通过 https://space.bilibili.com/3546939977107567 联系，并且关注我）\n";
		}
		else if (command == "SHUTDOWN") {
			cout << "正在关机……\n";
			system("shutdown /s /t 1");
		}
		else if (command == "RESTART") {
			cout << "正在重启……\n";
			system("shutdown /r /t 1");
		}
		else if (command == "SLEEP") {
			cout << "正在睡眠……\n";
			system("rundll32.exe powrprof.dll,SetSuspendState 0,1,0");
		}
		else if (command == "FW") {
			cout << "正在进入固件(BIOS/UEFI)……\n";
			system("shutdown /r /fw /t 1");
		}
		else if (command == "REC") {
			cout << "正在进入恢复环境(WinRE)……\n";
			system("shutdown /r /o /t 1");
		}
		else if (command == "CLS") {
			system("cls");
		}
		else if (command == "RUN") {
			cout << "请输入要执行的程序名称（包含后缀名）：";
			string programName;
			getline(cin, programName);
			if (CheckFileIsVaild(programName)) {
				RunProgram(programName);
			}
		}
		else if (command == "") {
			
		}
		else {
			if (SplitStrBySpace(command).size() > 1 && SplitStrBySpace(command)[0] == "EXITF") {
				string exitCodeStr = SplitStrBySpace(command)[1];
				try {
					int exitCode = stoi(exitCodeStr);
					exit(exitCode);
				}
				catch (const invalid_argument&) {
					cout << "无效的退出代码！请确保输入的是一个整数。\n";
				}
				catch (const out_of_range&) {
					cout << "退出代码超出范围！请确保输入的是一个有效的整数。\n";
				}
				catch (const exception& e) {
					cout << "发生错误：" << e.what() << endl;
				}
				catch (...) {
					cout << "发生未知错误！\n";
				}
			}
			else {
				cout << "无效的指令！\n";
			}
		}
	}
}
int main(int argc, char** argv) {
	bool debug = false;
	cout << "Loading......\n";
	for (int i = 1; i < argc; ++i) {
		if (string(argv[i]) == "--debug") {
			debug = true;
			break;
		}
	}
	ToolBox(argc, argv, debug);
	return 0;
}
