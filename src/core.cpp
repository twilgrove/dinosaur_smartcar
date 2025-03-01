#include "core.h"

void project(int signum)
{
    switch (signum)
    {
    case 0: // 程序初始化成功
        std::cout << "\33[33m" << PROGRAM_NAME << ":\33[0m init complete " << std::endl;
        break;
    case 2: // 程序被中断(Ctrl+C)
        std::cout << std::endl
                  << "\33[31m" << PROGRAM_NAME << ":\33[0mreceived \33[35mSIGINT\33[0m, process have been stopped, close resources..." << std::endl;
        close();
        break;
    case 1: // 程序正常结束
        std::cout << std::endl
                  << "\33[33m" << PROGRAM_NAME << ":\33[0m program is complete (^_^)" << std::endl;
        close();
        break;
    case -1: // 程序异常结束
        std::cout << std::endl
                  << "\33[31m" << PROGRAM_NAME << ":\33[0m process have been stopped..." << std::endl;
        close();
        break;
    }
}
