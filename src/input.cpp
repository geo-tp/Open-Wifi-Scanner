#include "input.h"

char cardputerInputHandler() {
    // Update keyboard state
    M5Cardputer.update();

    if (M5Cardputer.Keyboard.isChange()) {

        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

            if (status.enter) {
                return KEY_OK;
            }
            if(M5Cardputer.Keyboard.isKeyPressed(KEY_ARROW_LEFT)) {
                return KEY_ARROW_LEFT;
            }
            if(M5Cardputer.Keyboard.isKeyPressed(KEY_ARROW_RIGHT)) { 
                return KEY_ARROW_RIGHT;
            }
            if(M5Cardputer.Keyboard.isKeyPressed(KEY_ARROW_UP)) {
                return KEY_ARROW_UP;
            }
            if(M5Cardputer.Keyboard.isKeyPressed(KEY_ARROW_DOWN)) {
                return KEY_ARROW_DOWN;
            }
        }
        delay(10); // debounce
    }
    return KEY_NONE;
}
