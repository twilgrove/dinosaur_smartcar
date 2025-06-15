#include "headfile.h"

/* ----------------------------------------程序管理---------------------------------------- */

void project_manage(int signum)
{
    switch (signum)
    {
    case 2: // 程序被终端中断(Ctrl+C)
        std::cout << "process have been stopped, close resources..." << std::endl;
        rp.enable_or_disable(0);
        lp.enable_or_disable(0);
        r_fs.enable_or_disable(0);
        l_fs.enable_or_disable(0);
        ips200_clear();
        car.program_running = 0;
        break;
    case 1: // 程序正常结束
        std::cout << " program is completed" << std::endl;
        car.program_running = 0;
        break;
    case -1: // 程序异常结束
        std::cout << "process have been stopped..." << std::endl;
        car.program_running = 0;
        break;
    }
}

void reset(bool flag)
{
    if (flag)
    {
        ips200_clear();
        Camera.release();

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

void Update_time()
{
    gettimeofday(&car.now, NULL);

    car.total_seconds = (car.now.tv_sec - car.start.tv_sec) +
                        (car.now.tv_usec - car.start.tv_usec) / 1e6;
    car.minutes = (int)(car.total_seconds / 60);
    car.seconds = car.total_seconds - car.minutes * 60;

    // ips200_show_int(85, 50, car.minutes, 2);
    // ips200_show_string(104, 50, "m");
    // ips200_show_float(115, 50, car.seconds, 2, 2);
    // ips200_show_string(155, 50, "s");
}

void Test_period(const char *threadName)
{
    using namespace std::chrono;
    static steady_clock::time_point start = steady_clock::now();

    auto now = steady_clock::now();
    auto elapsed = duration_cast<milliseconds>(now - start);

    auto seconds = elapsed.count() / 1000;
    auto ms = elapsed.count() % 1000;

    std::cout << "[" << threadName << "] "
              << seconds << "s "
              << std::setw(3) << std::setfill(' ') << ms << "ms"
              << std::endl;

    start = now;
}