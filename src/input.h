#ifndef INPUT_H
#define INPUT_H

#include <M5Cardputer.h>

#define KEY_OK '\n'
#define KEY_DEL '\b'
#define KEY_NONE '\0'
#define KEY_RETURN '\r'
#define KEY_ARROW_UP ';'
#define KEY_ARROW_DOWN '.'
#define KEY_ARROW_LEFT ','
#define KEY_ARROW_RIGHT '/'

char cardputerInputHandler();

#endif