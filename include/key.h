#ifndef KEY_H
#define KEY_H

#include "GPIO.h"
void key();

class Key
{
public:
    Key(int gpioNum, std::string edge);
    Key(int gpioNum, int time_min, std::string edge);
    bool readValue();

private:
    GPIO gpio;
    int time_min;
    bool key_active;
    int key_s;
    uint8_t key_flag;
    std::string edge; // 按键类型: down:按下, up:松开
};
#endif
