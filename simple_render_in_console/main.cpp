// Include
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>
#include <cmath>

#include "renderSystem.h"
#include "input.h"

//Logics variables
bool isGameActive = true;
clock_t clockLastFrame = 0;

int framesCounter = 0;
float framesTimeCounter = 0;
int fps = 0;

double offset = 0;

// Functions 
void SetupSystem()
{
    srand(time(0));

    // Initialize render system
    RenderSystemInitialize();
}

void Initialize()
{
    // Set clockLastFrame start value
    clockLastFrame = clock();
}

void Render()
{
    // Start frame
    RenderSystemClear();

    // Draw frame (test)
    // VK_UP, VK_DOWN, VK_RIGHT, VK_LEFT являются виртуальными кодами клавиш со стрелками на клавиатуре
    if (IsKeyDown(VK_UP))
        RenderSystemDrawChar(10, 35, 24, ConsoleColor_Red, ConsoleColor_Gray);

    if (IsKeyDown(VK_DOWN))
        RenderSystemDrawChar(12, 35, 25, ConsoleColor_Blue, ConsoleColor_White);

    if (IsKeyDown(VK_RIGHT))
        RenderSystemDrawChar(12, 37, 26, ConsoleColor_Yellow, ConsoleColor_DarkGray);

    if (IsKeyDown(VK_LEFT))
        RenderSystemDrawChar(12, 33, 27, ConsoleColor_Green, ConsoleColor_DarkCyan);

    RenderSystemDrawText(8, 30, "Some text drawing test!", ConsoleColor_Green, ConsoleColor_Black);

    // Draw FPS
    char textBuffer[32];
    // Функция sprintf похожа на printf, но в отличие от неё записывает форматированную строку не в строку,
    // а в одномерный массив символов, переданный первым параметром.
    sprintf(textBuffer, "FPS: %d", fps);
    RenderSystemDrawText(1, 5, textBuffer, ConsoleColor_Yellow, ConsoleColor_Black);
    char textFps[40];
    sprintf(textFps, "CLOCKS: %i", CLOCKS_PER_SEC);
    RenderSystemDrawText(2, 5, textFps, ConsoleColor_Yellow, ConsoleColor_Black);

    // Draw sin
    if (IsKeyDown(VK_RIGHT))
        offset -= .5;
    if (IsKeyDown(VK_LEFT))
        offset += .5;

    for (double x = 0; x < screenColumns; x++)
    {
        double sin_x = sin(x);
        double sin_y = 1.5 * sin(x / 2 - offset) + 15;
        RenderSystemDrawChar(int(sin_y), x, 0xB2, ConsoleColor_Cyan, ConsoleColor_Black);

    }

    // End frame
    RenderSystemFlush();

}

void Update()
{
    // Calculate delta time
    // clock() функция из <time.h> возвращает кол-во тиков процесора прошедших со времени запуска программы
    clock_t clockNow = clock();
    clock_t deltaClock = clockNow - clockLastFrame;
    // CLOCKS_PER_SEC макрос который является кол-вом тиков процессора в секунду
    float deltaTime = float(deltaClock) / CLOCKS_PER_SEC;
    clockLastFrame = clockNow;

    // Calculate FPS
    ++framesCounter;
    framesTimeCounter += deltaTime;
    if (framesTimeCounter >= 1.0)
    {
        framesTimeCounter -= 1.0;
        fps = framesCounter;
        framesCounter = 0;
    }
}

void Shutdown()
{

}

int main()
{
    SetupSystem();
    Initialize();

    do
    {
        Render();
        Update();
    }
    while (isGameActive);

    Shutdown();
    return 0;
}


























