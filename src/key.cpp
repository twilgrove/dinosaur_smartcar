#include "key.h"
Key::Key(int gpioNum, Key_mode edge) : gpio(gpioNum, "in"), edge(edge)
{
    time_min = 5;
    key_s = 0;
    key_flag = 0;
    key_active = !gpio.readValue();
}

bool Key::readValue()
{
    if (key_flag == 1 && !gpio.readValue() == key_active && edge == up)
    {
        key_flag = 2;
        return true;
    }

    if (gpio.readValue() == key_active)
    {
        if (++key_s >= time_min && key_flag == 0)
        {
            key_s = 0;
            key_flag = 1;
            if (edge == down)
            {
                key_flag = 2;
                return true;
            }
        }
    }
    else
    {
        key_s = 0;
        key_flag = 0;
    }

    return false;
}
