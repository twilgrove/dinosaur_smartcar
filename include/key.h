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
    int time_min;
    bool key_active;
    int key_s;
    uint8_t key_flag;
    Key_mode edge; // 按键类型: down:按下, up:松开
};
#endif
