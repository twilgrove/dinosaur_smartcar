#include "../include/main.h"

int main()
{
    pwm_init(0, 4, 1, 1000000);
    pwm_set_duty_cycle(0, 0.5);

    return 0;
}