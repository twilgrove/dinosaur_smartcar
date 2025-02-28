#ifndef KEY_H
#define KEY_H

#include "GPIO.h"

class Key
{
public:
    enum Key_mode
    {
        up,
        down
    };
    Key(int gpioNum, Key_mode edge);
    ~Key() = default;
    bool readValue();

private:
    GPIO gpio;
    int time_min = 5; // 按键消抖时间
    bool key_active;
    int key_s = 0;
    uint8_t key_flag = 0;
    Key_mode edge; // 按键类型: down:按下, up:松开
};
#endif
