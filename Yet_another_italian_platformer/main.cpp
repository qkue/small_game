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
#include "level.h"
#include "unitData.h"

// Constants
const int maxUnitsCount = 35;

const float cellBeginValue = 0.001f;
const float cellEndValue = 0.999f;

//Logics variables
bool isGameActive = true;
clock_t clockLastFrame = 0;

int framesCounter = 0;
float framesTimeCounter = 0;
int fps = 0;

unsigned char levelData[levelRows][levelColumns];
UnitData unitsData[maxUnitsCount];
int unitsCount = 0;
int heroIndex = 0;

//double offset = 0;

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

    // Load level
    for (int r = 0; r < levelRows; ++r)
    {
        for (int c = 0; c < levelColumns; ++c)
        {
            unsigned char cellSymbol = levelData0[r][c];
            levelData[r][c] = cellSymbol;

            switch(cellSymbol)
            {
                case CellSymbol_Hero:
                {
                    heroIndex = unitsCount;
                }
                case CellSymbol_Goomba:
                {
                    UnitType unitType = GetUnitTypeFromCell(cellSymbol);
                    unitsData[unitsCount].type = unitType;
                    unitsData[unitsCount].y = float(r);
                    unitsData[unitsCount].x = float(c);
                    unitsData[unitsCount].health = 1;
                    unitsData[unitsCount].ySpeed = 0.0;
                    unitsData[unitsCount].xSpeed = 0.0;
                    unitsData[unitsCount].yOrder = UnitOrder_None;
                    unitsData[unitsCount].xOrder = UnitOrder_None;
                    unitsCount++;

                    break;
                }
            }
        }
    }

}

void Render()
{
    // Start frame
    RenderSystemClear();

    /*
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
    */

   // Draw level
   for (int r = 0; r < levelRows; ++r) 
   {
    for (int c = 0; c < levelColumns; ++c)
    {
        unsigned char cellSymbol = levelData[r][c];
        
        unsigned char renderSymbol = GetRenderCellSymbol(cellSymbol);
        ConsoleColor symbolColor = GetRenderCellSymbolColor(cellSymbol);
        ConsoleColor backgroundColor = GetRenderCellSymbolBackgroundColor(cellSymbol);

        RenderSystemDrawChar(r, c, renderSymbol, symbolColor, backgroundColor);
    }
   }

    // Draw FPS
    char textBuffer[32];
    // Функция sprintf похожа на printf, но в отличие от неё записывает форматированную строку не в строку,
    // а в одномерный массив символов, переданный первым параметром.
    sprintf(textBuffer, "FPS: %d", fps);
    RenderSystemDrawText(0, 0, textBuffer, ConsoleColor_Yellow, ConsoleColor_Black);
    char textFps[40];
    sprintf(textFps, "CLOCKS: %i", CLOCKS_PER_SEC);
    RenderSystemDrawText(0, 12, textFps, ConsoleColor_Yellow, ConsoleColor_Black);
  
    /*
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
    */

    // End frame
    RenderSystemFlush();

}

bool MoveUnitTo(UnitData *pointerToUnitData, float newX, float newY)
{
    // Ignore dead units
    if (pointerToUnitData->health <= 0)
    {
        return false;
    }

    bool canMoveToCell = false;

    int newRow = int(newY);
    int newColumn = int(newX);
    int oldRow = int(pointerToUnitData->y);
    int oldColumn = int(pointerToUnitData->x);

    unsigned char unitSymbol = levelData[oldRow][oldColumn];
    unsigned char destinationCellSymbol = levelData[newRow][newColumn];

    // All unit actions
    switch(destinationCellSymbol)
    {
        case CellSymbol_Empty:
        {
            canMoveToCell = true;
            break;
        }
    }

    if (canMoveToCell)
    {
        levelData[oldRow][oldColumn] = CellSymbol_Empty;
        pointerToUnitData->x = newX;
        pointerToUnitData->y = newY;

        levelData[newRow][newColumn] = unitSymbol;
    }

    return canMoveToCell;
}

void UpdateUnit(UnitData *pointerToUnitData, float deltaTime)
{
    // Unit row and column
    int row = int(pointerToUnitData->y);
    int column = int(pointerToUnitData->x);

    // Y Order
    if (pointerToUnitData->yOrder == UnitOrder_Backward)
    {
        // Jump
        if(      (pointerToUnitData->y >= (row + cellEndValue)) 
            &&   (levelData[row + 1][column] != CellSymbol_Empty)
            &&   (levelData[row + 1][column] != CellSymbol_Abyss) )
        {
            pointerToUnitData->ySpeed = -GetUnitJumpSpeed(pointerToUnitData->type);
        }     
    }

    // X Order
    if (pointerToUnitData->xOrder == UnitOrder_Backward)
    {
        pointerToUnitData->xSpeed = -GetUnitSpeedX(pointerToUnitData->type);
    } 
    else 
    {
        if (pointerToUnitData->xOrder == UnitOrder_Forward)
        {
            pointerToUnitData->xSpeed = GetUnitSpeedX(pointerToUnitData->type);
        }
        else
        {
            pointerToUnitData->xSpeed = 0;
        }
    }

    // New position
    float deltaY = pointerToUnitData->ySpeed * deltaTime;
    float deltaX = pointerToUnitData->xSpeed * deltaTime;
    float newY = pointerToUnitData->y + deltaY;
    float newX = pointerToUnitData->x + deltaX;
    int newRow = int(newY);
    int newColumn = int(newX);

    // Y(row) step
    if (newRow != row)
    {
        // If unit can go to cell
        if (MoveUnitTo(pointerToUnitData, pointerToUnitData->x, newY))
        {
            row = int(pointerToUnitData->y);
        }
        else
        {
            // Can not move cell down
            if (newRow > row)
            {
                pointerToUnitData->y = row + cellEndValue;
                if (pointerToUnitData->ySpeed > 0.0)
                {
                    pointerToUnitData->ySpeed = 0.0;
                }
            }
            // Can not move cell up
            else
            {
                pointerToUnitData->y = row + cellBeginValue;
                if (pointerToUnitData->ySpeed < 0.0)
                {
                    pointerToUnitData->ySpeed = 0.0;
                }
            }
        }
    }
    else
    {
        pointerToUnitData->y = newY;
    }

    // X(column) step
    if (newColumn != column)
    {
        // If unit can go to cell
        if (MoveUnitTo(pointerToUnitData, newX, pointerToUnitData->y))
        {
            column = int(pointerToUnitData->x);
        }
        else
        {
            // Can not move cell right
            if (newColumn > column)
            {
                pointerToUnitData->x = column + cellEndValue;
                
                if(pointerToUnitData->xSpeed > 0.0)
                {
                    pointerToUnitData->xSpeed = 0.0;
                }
            }
            // Can not move cell left
            else
            {
                pointerToUnitData->x = column + cellBeginValue;
                if (pointerToUnitData->xSpeed < 0.0)
                {
                    pointerToUnitData->xSpeed = 0.0;
                }
            }
        }
    }
    else
    {
        pointerToUnitData->x = newX;
    }

    // Gravity
    pointerToUnitData->ySpeed += gravitySpeed * deltaTime;
    if (pointerToUnitData->ySpeed > gravitySpeed)
    {
        pointerToUnitData->ySpeed = gravitySpeed;
    }

}

void Update()
{
    // Calculate delta time
    // clock() функция из <time.h> возвращает кол-во тиков процесора прошедших со времени запуска программы
    clock_t clockNow = clock();
    clock_t deltaClock = clockNow - clockLastFrame;
    // CLOCKS_PER_SEC макрос который является кол-вом тиков процессора в секунду
    float deltaTime = float(deltaClock) / (CLOCKS_PER_SEC);
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

    // Hero control
    if (IsKeyDown(VK_UP)) 
    {
        unitsData[heroIndex].yOrder = UnitOrder_Backward;
    }
    else
    {
        unitsData[heroIndex].yOrder = UnitOrder_None;
    }

    if (IsKeyDown(VK_LEFT))
    {
        unitsData[heroIndex].xOrder = UnitOrder_Backward;
    }
    else
    {
        if (IsKeyDown(VK_RIGHT))
        {
            unitsData[heroIndex].xOrder = UnitOrder_Forward;
        }
        else
        {
            unitsData[heroIndex].xOrder = UnitOrder_None;
        }
    }

    // Update all units
    for (int u = 0; u < unitsCount; ++u)
    {
        UpdateUnit(&unitsData[u], deltaTime);
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


























