#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

#define APP_VERSION "1.0.0"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "JiYuKiller v" << APP_VERSION << endl;
        cerr << "用法: JiYuKiller.exe <进程名>" << endl;
        return 1;
    }

    string processName = argv[1];
    string command = "taskkill /F /IM " + processName;
    cout << "正在执行: " << command << endl;

    int result = system(command.c_str());
    if (result == 0) {
        cout << "成功终止进程 " << processName << endl;
    }
    else {
        cerr << "终止进程失败，错误代码: " << result << endl;
    }
    return result;
}