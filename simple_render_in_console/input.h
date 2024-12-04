#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

// include
#include <Windows.h>

// Functions
bool IsKeyDown(int virtualKeyCode)
{
    // GetAsyncKeyState �� ���������� Windows.h ��������� �������� ��������� ������� �� ���������� � ����
    // short ����� (������������� ���, ���������� 2 �����).
    short keyState = GetAsyncKeyState(virtualKeyCode);
    // 0x8000 ��� 32768 � ���������� ������� ��� 10000000 00000000 � ��������
    return ( (keyState & 0x8000) > 0);
}


#endif // INPUT_H_INCLUDED
