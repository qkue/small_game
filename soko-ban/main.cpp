#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>

//settings
const int rowsCount = 10;
const int ColumnsCount = 15;

const unsigned char symbolHero = 2;
const unsigned char symbolWall = 177;
const unsigned char symbolBox = 254;
const unsigned char symbolExit = 176;

const unsigned char levelData0[rowsCount][ColumnsCount + 1] = {
        "#####2#########",
        "#  X   #   X ##",
        "# X ### X  #  #",
        "#X X  X  ## X #",
        "# X    ##  #  #",
        "#######    # X#",
        "#   X   XX #X #",
        "#XXX # # X   ##",
        "#1 X #   X X  #",
        "###############",
};

//logics
HANDLE consoleHandle = 0;
bool isGameActive = true;
unsigned char levelData[rowsCount][ColumnsCount];
int heroRow = 0;
int heroColumn = 0;


//funcs
void SetupSystem() {
    srand (time(0));
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Initialise() {
    //load level
    for (int r = 0; r < rowsCount; ++r) {
        for (int c = 0; c < ColumnsCount; ++c) {
            unsigned char symbol = levelData0[r][c];

            switch (symbol) {
            case '#':{
                levelData[r][c] = symbolWall;
                break;
            }

            case '1': {
                levelData[r][c] = symbolHero;
                heroRow = r;
                heroColumn = c;
                break;
            }

            case '2': {
                levelData[r][c] = symbolExit;
                break;
            }

            case 'X': {
                levelData[r][c] = symbolBox;
                break;
            }

            default: {
                levelData[r][c] = symbol;
                break;
            }
            }
        }
    }
}

void Render() {
    //system("cls");
    COORD cursorCoord;
    cursorCoord.X = 0;
    cursorCoord.Y = 0;
    SetConsoleCursorPosition(consoleHandle, cursorCoord);

    printf("\n\n\t");
    for (int r = 0; r < rowsCount; ++r) {
        for (int c = 0; c < ColumnsCount; ++c) {
            unsigned char symbol = levelData[r][c];

            switch(symbol) {
                case symbolWall: {
                    SetConsoleTextAttribute(consoleHandle, 15);
                    break;
                }
                case symbolHero: {
                    SetConsoleTextAttribute(consoleHandle, 10);
                    break;
                }
                case symbolBox: {
                    SetConsoleTextAttribute(consoleHandle, 14);
                    break;
                }
                case symbolExit: {
                    SetConsoleTextAttribute(consoleHandle, 12);
                    break;
                }
            }
            printf("%c", symbol);
        }
        printf("\n\t");
    }
    SetConsoleTextAttribute(consoleHandle, 7);
    printf("\n\n\tUse AWSD to move ");
    SetConsoleTextAttribute(consoleHandle, 10);
    printf("Hero");
    SetConsoleTextAttribute(consoleHandle, 7);
    printf(". R - Restart level.");
}

void MoveHeroTo (int row, int column) {
    unsigned char destinationCell = levelData[row][column];
    bool canMoveToCell = false;

    switch (destinationCell) {
        case ' ': {
            canMoveToCell = true;
            break;
        }

        case symbolExit: {
            isGameActive = false;
            break;
        }

        case symbolBox: {
            int heroDirectionR = row - heroRow;
            int heroDirectionC = column - heroColumn;

            if (levelData[row + heroDirectionR][column + heroDirectionC] == ' ') {
                canMoveToCell = true;
                levelData[row][column] = ' ';
                levelData[row + heroDirectionR][column + heroDirectionC] = symbolBox;
            }
            break;
        }
    }

    if (canMoveToCell) {
        levelData[heroRow][heroColumn] = ' ';
        heroRow = row;
        heroColumn = column;
        levelData[heroRow][heroColumn] = symbolHero;
    }
}

void Update() {
    unsigned char inputChar = _getch();
    inputChar = tolower(inputChar);

    switch (inputChar) {
        case 'w': {
            MoveHeroTo (heroRow - 1, heroColumn);
            break;
        }

        case 's': {
            MoveHeroTo (heroRow + 1, heroColumn);

            break;
        }

        case 'a': {
            MoveHeroTo (heroRow, heroColumn - 1);

            break;
        }

        case 'd': {
            MoveHeroTo (heroRow, heroColumn + 1);

            break;
        }

        case 'r': {
            Initialise();
            break;
        }
    }
}

void Shutdown() {
    system("cls");
    printf("\n\fPress any key to continue...");
    _getch();

}

int main() {
    SetupSystem();
    Initialise();

    do {
        Render();
        Update();
    }
    while (isGameActive);

    Shutdown();
    return 0;
}
