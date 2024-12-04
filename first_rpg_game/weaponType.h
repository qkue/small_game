#ifndef WEAPONTYPE_H_INCLUDED
#define WEAPONTYPE_H_INCLUDED

#include <vector>
#include "level.h"


enum WeaponType
{
    WeaponType_None,
    WeaponType_Fist,
    WeaponType_Stick,
    WeaponType_Club,
    WeaponType_Spear,
    WeaponType_Saber,
    WeaponType_Chain,
    WeaponType_Scythe,
    WeaponType_Claymor
};

const char* WeaponName_None          = "None";
const char* WeaponName_Fist          = "Fist";
const char* WeaponName_Stick         = "Stick";
const char* WeaponName_Club          = "Club";
const char* WeaponName_Spear         = "Spear";
const char* WeaponName_Saber         = "Saber";
const char* WeaponName_Chain         = "Chain";
const char* WeaponName_Scythe        = "Scythe";
const char* WeaponName_Claymor       = "Claymor";


const char* GetWeaponName(WeaponType weaponType)
{
    switch(weaponType)
    {
        case WeaponType_Fist:       return WeaponName_Fist;
        case WeaponType_Stick:      return WeaponName_Stick;
        case WeaponType_Club:       return WeaponName_Club;
        case WeaponType_Spear:      return WeaponName_Spear;
        case WeaponType_Chain:      return WeaponName_Chain;
        case WeaponType_Scythe:     return WeaponName_Scythe;
        case WeaponType_Claymor:    return WeaponName_Claymor;
    }
    return WeaponName_None;
}

std::vector<int> GetWeaponDamage(WeaponType weaponType)
{
    switch(weaponType)
    {
        case WeaponType_Fist:
        {
            std::vector <int> temp = {1, 2};
            return temp;
        }
        case WeaponType_Stick:
        {
            std::vector <int> temp1 = {12, 16};
            return temp1;
        }
        case WeaponType_Club:
        {
            std::vector <int> temp2 = {20, 24};
            return temp2;
        }
        case WeaponType_Spear:
        {
            std::vector <int> temp3 = {26, 32};
            return temp3;
        }
        case WeaponType_Saber:
        {
            std::vector <int> temp4 = {34, 40};
            return temp4;
        }
        case WeaponType_Chain:
        {
            std::vector <int> temp5 = {42, 48};
            return temp5;
        }
        case WeaponType_Scythe:
        {
            std::vector <int> temp6 = {50, 60};
            return temp6;
        }
        case WeaponType_Claymor:
        {
            std::vector <int> temp7 = {62, 100};
                return temp7;
        }
    }
    std::vector <int> temp8 = {0, 0};
    return temp8;
}

WeaponType GetWeaponTypeFromCell(unsigned char cellSymbol)
{
    switch(cellSymbol)
    {
        case CellSymbol_Stick:      return WeaponType_Stick;
        case CellSymbol_Club:       return WeaponType_Club;
        case CellSymbol_Spear:      return WeaponType_Spear;
        case CellSymbol_Saber:      return WeaponType_Saber;
        case CellSymbol_Chain:      return WeaponType_Chain;
        case CellSymbol_Scythe:     return WeaponType_Scythe;
        case CellSymbol_Claymor:    return WeaponType_Claymor;
    }
    return WeaponType_None;
}


#endif // WEAPONTYPE_H_INCLUDED


























