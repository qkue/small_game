#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include "consoleColor.h"

// Constants
const int rowsCount = 15;
const int columnCount = 35;
const int heartHeal = 100;

const unsigned char fogOfWarRenderSymbol = 176;
ConsoleColor fogOfWarRenderColor = ConsoleColor_DarkGray;

const unsigned char CellSymbol_Empty    = ' ';
const unsigned char CellSymbol_Wall     = '#';
const unsigned char CellSymbol_Hero     = 'h';
const unsigned char CellSymbol_Exit     = 'e';
const unsigned char CellSymbol_Orc      = 'o';
const unsigned char CellSymbol_Skeleton = 's';
const unsigned char CellSymbol_Skeleton_King = 'k';
const unsigned char CellSymbol_Stick    = '1';
const unsigned char CellSymbol_Club     = '2';
const unsigned char CellSymbol_Spear    = '3';
const unsigned char CellSymbol_Saber    = '4';
const unsigned char CellSymbol_Heart    = 'H';
const unsigned char CellSymbol_Chain    = 'C';
const unsigned char CellSymbol_Scythe   = 'S';
const unsigned char CellSymbol_Claymor  = 'L';

const int GameLevel = 2;

const unsigned char levelData0[GameLevel][rowsCount][columnCount + 1] =
{

    {

        "###################################",
        "# H # ##      o           s       #",
        "# # ok #o############## ######### #",
        "# ###### #   #C#3  s  # ##s##  H# #",
        "#  s       # # ###### # #s4   #s# #",
        "###### # ### #      # # ##s##   # #",
        "## Ho# # # # #### #H#   ##### # # #",
        "#S #   # # o    # ## ######H# L # #",
        "######## # ####         #  s# # #s#",
        "#  2#    #      ####### # # #   # #",
        "# ### #### ######   o   # #   #   #",
        "#  o       #      ####### #########",
        "############ ######   #1# # s #   #",
        "#h                  #   #   #s  # #",
        "#######e#########################e#",
    },
    {
        "###################################",
        "#                                 #",
        "#                                 #",
        "#                                 #",
        "#                                 #",
        "#                                 #",
        "#                                 #",
        "#                   h             #",
        "#                                 #",
        "#                                 #",
        "#                                 #",
        "#                                 #",
        "#                                 #",
        "#                                e#",
        "###################################",
    }
};


unsigned char GetRenderCellSymbol(unsigned char cellSymbol)
{
    switch(cellSymbol)
    {
        case CellSymbol_Empty:      return ' ';
        case CellSymbol_Wall:       return 177;
        case CellSymbol_Hero:       return 2;
        case CellSymbol_Orc:        return 2;
        case CellSymbol_Skeleton:   return 2;
        case CellSymbol_Stick:      return 47;
        case CellSymbol_Club:       return 33;
        case CellSymbol_Spear:      return 24;
        case CellSymbol_Saber:      return 108;
        case CellSymbol_Exit:       return 178;
        case CellSymbol_Heart:      return 3;
        case CellSymbol_Chain:      return 64;
        case CellSymbol_Scythe:     return 251;
        case CellSymbol_Claymor:    return 197;
        case CellSymbol_Skeleton_King: return 240;
    }
    return '?';
}

ConsoleColor GetRenderCellSymbolColor(unsigned char cellSymbol)
{
    switch(cellSymbol)
    {
        case CellSymbol_Empty:      return ConsoleColor_Black;
        case CellSymbol_Wall:       return ConsoleColor_White;
        case CellSymbol_Hero:       return ConsoleColor_Yellow;
        case CellSymbol_Orc:        return ConsoleColor_Green;
        case CellSymbol_Skeleton:   return ConsoleColor_White;
        case CellSymbol_Stick:      return ConsoleColor_DarkYellow;
        case CellSymbol_Club:       return ConsoleColor_DarkRed;
        case CellSymbol_Spear:      return ConsoleColor_DarkCyan;
        case CellSymbol_Saber:      return ConsoleColor_Cyan;
        case CellSymbol_Exit:       return ConsoleColor_DarkRed;
        case CellSymbol_Heart:      return ConsoleColor_Red;
        case CellSymbol_Chain:      return ConsoleColor_DarkBlue;
        case CellSymbol_Scythe:     return ConsoleColor_Cyan;
        case CellSymbol_Claymor:    return ConsoleColor_DarkMagenta;
        case CellSymbol_Skeleton_King: return ConsoleColor_Red;
    }
    return ConsoleColor_Gray;
}

#endif // LEVEL_H_INCLUDED
