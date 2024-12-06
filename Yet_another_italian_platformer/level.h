#pragma once

// Include
#include "renderSystem.h"

// Constants
const int levelRows = screenRows;
const int levelColumns = screenColumns;

const int gravitySpeed = 38;

const unsigned char CellSymbol_Empty        = ' ';
const unsigned char CellSymbol_Brickwall    = '#';
const unsigned char CellSymbol_SteelWall    = '@';
const unsigned char CellSymbol_