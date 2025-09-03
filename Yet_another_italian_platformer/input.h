#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

// include
#include <Windows.h>

// Functions
bool IsKeyDown(int virtualKeyCode)
{
    // GetAsyncKeyState из библиотеки Windows.h позволяет получить состояние клавиши на клавиатуре в виде
    // short числа (целочисленный тип, занимающий 2 байта).
    short keyState = GetAsyncKeyState(virtualKeyCode);
    // 0x8000 это 32768 в десятичной системе или 10000000 00000000 в двоичной
    return ( (keyState & 0x8000) > 0);
}


#endif // INPUT_H_INCLUDED
