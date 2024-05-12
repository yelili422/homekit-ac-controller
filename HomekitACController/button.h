#pragma once

#include <Arduino.h>

typedef void (*event_handler_t)();

class Button
{
public:
    Button(int pin, bool pullup = false) : pin(pin), state(HIGH), pullup(pullup)
    {
        pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
    };
    void loop();
    inline void on_click(event_handler_t handler)
    {
        click_handler = handler;
    };

private:
    int pin;
    bool pullup;
    int state;
    event_handler_t click_handler;
};
