#ifndef UNITDATA_H_INCLUDED
#define UNITDATA_H_INCLUDED

#include "unitType.h"
#include "weaponType.h"

struct UnitData
{
    UnitType type;
    int row;
    int column;
    int health;
    WeaponType weapon;
};

#endif // UNITDATA_H_INCLUDED
