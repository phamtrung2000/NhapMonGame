#include "Weapon.h"

Weapon::Weapon() : CGameObject()
{
	Category = CATEGORY::WEAPON;
	isMarioWeapon = false;
	StartX = StartY = 0;
}
