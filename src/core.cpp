#include "core.h"

/* ----------------------------------------全局变量---------------------------------------- */
extern std::atomic<bool> running;
extern Key key1;

/* ----------------------------------------程序管理---------------------------------------- */
void run()
{
    while (running)
    {
        if (key1.readValue())
            break;
        else
            usleep(10000);
    }
}

bool reset()
{
    std::cout << "\33[33m" << PROGRAM_NAME << ":\33[0m program reset..." << std::endl;

    // 获取程序的执行路径
    const char *program = "/proc/self/exe";
    char buffer[1024];

    ssize_t len = readlink(program, buffer, sizeof(buffer) - 1);
    if (len == -1)
    {
        std::cerr << "Error getting the program path: " << strerror(errno) << std::endl;
        return false; // 如果发生错误，返回 false
    }
    buffer[len] = '\0'; // Null-terminate the string

    // 使用 exec() 函数重新启动程序
    std::cout << "Restarting the program..." << std::endl;

    char *const args[] = {nullptr}; // 删除未使用的 env

    if (execv(buffer, args) == -1)
    {
        std::cerr << "Error restarting the program: " << strerror(errno) << std::endl;
        return false; // 如果 execv 失败，返回 false
    }

    return true;
}

void project(int signum)
{
    switch (signum)
    {
    case 2: // 程序被终端中断(Ctrl+C)
        std::cout << std::endl
                  << "\33[31m" << PROGRAM_NAME << ":\33[0mreceived \33[35mSIGINT\33[0m, process have been stopped, close resources..." << std::endl;
        running = false;
        break;
    case 1: // 程序正常结束
        std::cout << std::endl
                  << "\33[33m" << PROGRAM_NAME << ":\33[0m program is complete (^_^)" << std::endl;
        running = false;
        break;
    case -1: // 程序异常结束
        std::cout << std::endl
                  << "\33[31m" << PROGRAM_NAME << ":\33[0m process have been stopped..." << std::endl;
        running = false;
        break;
    }
}
