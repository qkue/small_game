/* Определяет координаты символьной ячейки в
    буфере экрана консоли. Источник системы координат
    (0,0) находится в верхней левой ячейке буфера. */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>
#include <vector>

#include "consoleColor.h"
#include "level.h"
#include "weaponType.h"
#include "unitType.h"
#include "unitData.h"

const int maxUnitsCount = 35;

HANDLE consoleHandle = 0;
bool isGameActive = true;

unsigned char levelData[rowsCount][columnCount];
bool fogOfWar[rowsCount][columnCount];

UnitData unitsData[maxUnitsCount];
int unitsCount = 0;
int heroIndex = 0;
int level = 0;

char tempBuffer[128];
char statusMessage[200];

void SetupSystem() {
    statusMessage[0] = 0;

    srand (time(0));

    consoleHandle = GetStdHandle (STD_OUTPUT_HANDLE);

    /* дескриптор с передачей в него макроса STD_OUTPUT_HANDLE

    GetStdHandle
        Извлекает дескриптор для указанного стандартного
        устройства (стандартный ввод, стандартный вывод
        или стандартная ошибка).

    STD_OUTPUT_HANDLE
    	Стандартное выходное устройство. Изначально это активный буфер экрана консоли, CONOUT$.
    */


    CONSOLE_CURSOR_INFO cursorInfo;
    /*скрывает мигающий курсор консоли*/

    cursorInfo.dwSize = 1;
    /* Процент ячейки символа, заполненной курсором.
    Это значение составляет от 1 до 100. Внешний вид
    курсора зависит от полного заполнения ячейки до
    отображения в виде горизонтальной линии в нижней
    части ячейки. */

    cursorInfo.bVisible = 0;
    /* Видимость курсора. Если курсор виден, этот элемент
    имеет значение TRUE. */

    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    /* Задает размер и видимость курсора для указанного буфера экрана консоли. */

}

void RevealFogOfWar (int row, int column)
{
    for (int r = row - 1; r <= row + 1; ++r)
    {
        for (int c = column - 1; c <= column + 1; ++c)
        {
            fogOfWar[r][c] = false;
        }
    }
}

void Initialize()
{
    unitsCount = 0;

    for (int r = 0; r < rowsCount; ++r)
    {
        for (int c = 0; c < columnCount; ++c)
        {
            fogOfWar[r][c] = true;

            unsigned char cellSymbol = levelData0[level][r][c];
            levelData[r][c] = cellSymbol;

            switch(cellSymbol)
            {
                case CellSymbol_Hero:
                    {
                        heroIndex = unitsCount;
                    }
                case CellSymbol_Orc:
                case CellSymbol_Skeleton:
                case CellSymbol_Skeleton_King:
                {
                    UnitType unitType = GetUnitTypeFromCell(cellSymbol);
                    unitsData[unitsCount].type = unitType;
                    unitsData[unitsCount].row = r;
                    unitsData[unitsCount].column = c;
                    unitsData[unitsCount].weapon = GetUnitDefaultWeapon(unitType);
                    unitsData[unitsCount].health = GetUnitDefaultHealth(unitType);
                    unitsCount++;

                    break;
                }
            }
        }
    }

    // Reveal fog of war;
    RevealFogOfWar(unitsData[heroIndex].row, unitsData[heroIndex].column);

}

void Render() {
    COORD cursorCoord;
    /* Определяет координаты символьной ячейки в
    буфере экрана консоли. Источник системы координат
    (0,0) находится в верхней левой ячейке буфера. */

    cursorCoord.X = 0;
    cursorCoord.Y = 0;
    SetConsoleCursorPosition(consoleHandle, cursorCoord);
    /* Задает позицию курсора в указанном буфере экрана консоли. */

    // Draw game title;
    SetConsoleTextAttribute(consoleHandle, ConsoleColor_Green);
    printf("\n\tDUNGEONS AND ORCS");

    // Draw Hero HP;
    SetConsoleTextAttribute(consoleHandle, ConsoleColor_Red);
    printf("\n\n\tHP: ");
    SetConsoleTextAttribute(consoleHandle, ConsoleColor_White);
    printf("%i ", unitsData[heroIndex].health);

    // Draw Hero Weapon;
    SetConsoleTextAttribute(consoleHandle, ConsoleColor_Cyan);
    printf("    Weapon: ");
    SetConsoleTextAttribute(consoleHandle, ConsoleColor_Green);
    printf("%s ", GetWeaponName(unitsData[heroIndex].weapon));
    SetConsoleTextAttribute(consoleHandle, ConsoleColor_Magenta);
    printf(" (Dmg: %i - %i)            ", GetWeaponDamage(unitsData[heroIndex].weapon)[0], GetWeaponDamage(unitsData[heroIndex].weapon)[1]);

    // Draw level;
    printf("\n\n\t");
    for (int r = 0; r < rowsCount; ++r)
    {
        for (int c = 0; c < columnCount; ++c)
        {
            if (fogOfWar[r][c] == false)
            {
                unsigned char cellSymbol = levelData[r][c];
                unsigned char renderCellSymbol = GetRenderCellSymbol(cellSymbol);
                ConsoleColor cellColor = GetRenderCellSymbolColor(cellSymbol);

                SetConsoleTextAttribute(consoleHandle, cellColor);
                printf("%c", renderCellSymbol);
            }
            else
            {
                SetConsoleTextAttribute(consoleHandle, fogOfWarRenderColor);
                printf("%c", fogOfWarRenderSymbol);
            }

        }
        printf("\n\t");
    }

    // Fill status message with spaces;
    while (strlen(statusMessage) < (sizeof(statusMessage) - 1))
    {
        strcat(statusMessage, " ");
    }
    SetConsoleTextAttribute(consoleHandle, ConsoleColor_Yellow);
    printf("\n\n\t%s", statusMessage);

    // Clear status message;
    statusMessage[0] = 0;
}

void MoveUnitTo(UnitData* pointerToUnitData, int row, int column)
{
    // Ignore dead units;
    if (pointerToUnitData->health <= 0)
    {
        return;
    }

    unsigned char unitSymbol = levelData[pointerToUnitData->row][pointerToUnitData->column];
    unsigned char destinationCellSymbol = levelData[row][column];
    bool canMoveToCell = false;

    switch(destinationCellSymbol)
    {
        case CellSymbol_Empty:
        {
            canMoveToCell = true;
            break;
        }

        case CellSymbol_Hero:
        case CellSymbol_Orc:
        case CellSymbol_Skeleton:
        case CellSymbol_Skeleton_King:
            {
                UnitType destinationUnitType = GetUnitTypeFromCell(destinationCellSymbol);
                if ((pointerToUnitData->type == UnitType_Hero || destinationUnitType == UnitType_Hero) && pointerToUnitData->type != destinationUnitType)
                {
                    for (int u =0; u < unitsCount; ++u)
                    {
                        if (unitsData[u].health <= 0)
                        {
                            continue;
                        }

                        if (unitsData[u].row == row && unitsData[u].column == column)
                        {
                            int damage = rand() % (GetWeaponDamage(pointerToUnitData->weapon)[1] - GetWeaponDamage(pointerToUnitData->weapon)[0] + 1) + GetWeaponDamage(pointerToUnitData->weapon)[0];
                            unitsData[u].health -= damage;

                            sprintf(tempBuffer, " %s dealt %i damage to %s", GetUnitName(pointerToUnitData->type), damage, GetUnitName(destinationUnitType));
                            strcat(statusMessage, tempBuffer);

                            if (unitsData[u].health <= 0.0f)
                            {
                                levelData[row][column] = CellSymbol_Empty;
                            }

                            break;
                        }
                    }
                }
                break;
            }
    }

    // Only hero actions;
    if (pointerToUnitData->type == UnitType_Hero)
    {
        switch(destinationCellSymbol)
        {
            // Weapon Cell;
            case CellSymbol_Stick:
            case CellSymbol_Club:
            case CellSymbol_Spear:
            case CellSymbol_Saber:
            case CellSymbol_Chain:
            case CellSymbol_Claymor:
            case CellSymbol_Scythe:
            {
                canMoveToCell = true;

                WeaponType weaponType = GetWeaponTypeFromCell(destinationCellSymbol);
                if (unitsData[heroIndex].weapon < weaponType)
                {
                    unitsData[heroIndex].weapon = weaponType;
                }
                break;
            }

            case CellSymbol_Heart:
            {
                canMoveToCell = true;
                if (unitsData[heroIndex].health + heartHeal > GetUnitDefaultHealth(UnitType_Hero))
                {
                    unitsData[heroIndex].health = GetUnitDefaultHealth(UnitType_Hero);
                }
                else
                {
                    unitsData[heroIndex].health += heartHeal;
                }
                break;
            }

            // Exit cell;
            case CellSymbol_Exit:
            {
                if (level != GameLevel - 1)
                {
                    ++level;
                    UnitData heroDataFromPrevLevel = unitsData[heroIndex];
                    //heroTransferHealth = unitsData[heroIndex].health;
                    //heroTransferWeapon = unitsData[heroIndex].weapon;
                    Initialize();
                    unitsData[heroIndex].health = heroDataFromPrevLevel.health;
                    unitsData[heroIndex].weapon = heroDataFromPrevLevel.weapon;
                    break;
                }
                else
                {
                    isGameActive = false;
                    break;
                }

            }
        }
    }

    if (canMoveToCell)
    {
        // Remove unit symbol from previous position;
        levelData[pointerToUnitData->row][pointerToUnitData->column] = CellSymbol_Empty;

        // Set new hero position;
        pointerToUnitData->row = row;
        pointerToUnitData->column = column;

        // Set hero symbol to new position;
        levelData[pointerToUnitData->row][pointerToUnitData->column] = unitSymbol;

        if (pointerToUnitData->type == UnitType_Hero)
        {
            RevealFogOfWar(row, column);
        }
    }

}
void UpdateAI()
{
    // Pass all units;
    for (int u = 0; u < unitsCount; ++u)
    {
        // Ignore Hero;
        if (u == heroIndex)
        {
            continue;
        }

        // Ignore dead units;
        if (unitsData[u].health <= 0)
        {
            continue;
        }

        // Distance to hero;
        int distanceToHeroR = abs(unitsData[heroIndex].row - unitsData[u].row);
        int distanceToHeroC = abs(unitsData[heroIndex].column - unitsData[u].column);

        // If hero nearby;
        if ((distanceToHeroC + distanceToHeroR) == 1)
        {
            // Attack Hero;
            MoveUnitTo(&unitsData[u], unitsData[heroIndex].row, unitsData[heroIndex].column);
        }
        else
        {
            // Random move;
            switch(rand() % 4)
            {
                case 0:
                {
                    MoveUnitTo(&unitsData[u], unitsData[u].row - 1, unitsData[u].column);
                    break;
                }

                case 1:
                {
                    MoveUnitTo(&unitsData[u], unitsData[u].row + 1, unitsData[u].column);
                    break;
                }

                case 2:
                {
                    MoveUnitTo(&unitsData[u], unitsData[u].row, unitsData[u].column - 1);
                    break;
                }

                case 3:
                {
                    MoveUnitTo(&unitsData[u], unitsData[u].row, unitsData[u].column + 1);
                    break;
                }
            }
        }
    }
}


void Update() {
    unsigned char inputChar = _getch();
    inputChar = tolower(inputChar);

    switch(inputChar)
    {
        // Up;
        case 'w':
        {
            MoveUnitTo(&unitsData[heroIndex], unitsData[heroIndex].row - 1, unitsData[heroIndex].column);
            break;
        }

        // Down;
        case 's':
        {
            MoveUnitTo(&unitsData[heroIndex], unitsData[heroIndex].row + 1, unitsData[heroIndex].column);
            break;
        }

        // Left;
        case 'a':
        {
            MoveUnitTo(&unitsData[heroIndex], unitsData[heroIndex].row, unitsData[heroIndex].column - 1);
            break;
        }

        case 'd':
        {
            MoveUnitTo(&unitsData[heroIndex], unitsData[heroIndex].row, unitsData[heroIndex].column + 1);
            break;
        }

        case 'r':
        {
            Initialize();
            break;
        }
    }

    // AI turn;
    UpdateAI();

    // Hero death;
    if (unitsData[heroIndex].health <= 0)
    {
        isGameActive = false;
    }
    else
    {
        // Health regen;
        if (unitsData[heroIndex].health < GetUnitDefaultHealth(UnitType_Hero))
        {
            unitsData[heroIndex].health++;
        }
    }

}

void Shutdown() {
    system("cls");
    printf("\n\tGame over...");
    _getch();
}

int main() {
    SetupSystem();
    Initialize();

    do {
        Render();
        Update();
    } while (isGameActive);

    Shutdown();
    return 0;
}




















