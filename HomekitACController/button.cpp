#include "button.h"

void Button::loop()
{
    int new_state = digitalRead(pin);

    if (new_state == LOW) {
        state = LOW;
    }

    if (state == LOW && new_state == HIGH) {
        if (click_handler) {
            printf("on button click: \n");
            click_handler();
        }
    }

    state = new_state;
}
