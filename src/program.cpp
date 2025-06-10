#include "image_deal.h"
#include "thread.h"
#include "isr.h"
#include "key_board.h"
#include "PID.h"
#include "my_control.h"
#include "traffic_circle.h"
#include "headfile.h"
void close()
{
    running = 0;
}
/* ----------------------------------------程序管理---------------------------------------- */

void project_manage(int signum)
{
    switch (signum)
    {
        case 2: // 程序被终端中断(Ctrl+C)
            std::cout << "process have been stopped, close resources..." << std::endl;
            close();
            break;
        case 1: // 程序正常结束
            std::cout << " program is complete" << std::endl;
            break;
        case -1: // 程序异常结束
            std::cout << "process have been stopped..." << std::endl;
            break;
    }

    //close();
}

void reset(bool flag)
{
    if (flag)
    {
        std::cout << "\33[33m" << PROGRAM_NAME << ":\33[0m program reset..." << std::endl;

        // 获取程序的执行路径
        const char *program = "/proc/self/exe";
        char buffer[1024];

        ssize_t len = readlink(program, buffer, sizeof(buffer) - 1);
        if (len == -1)
        {
            std::cerr << "Error getting the program path: " << strerror(errno) << std::endl;
            return;
        }
        buffer[len] = '\0'; // Null-terminate the string

        // 使用 exec() 函数重新启动程序
        std::cout << "Restarting the program..." << std::endl;

        char *const args[] = {nullptr}; // 删除未使用的 env

        if (execv(buffer, args) == -1)
        {
            std::cerr << "Error restarting the program: " << strerror(errno) << std::endl;
            return;
        }

        return;
    }
}