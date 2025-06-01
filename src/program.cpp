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

