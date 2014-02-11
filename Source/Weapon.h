//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Bullet.h"

class Ship;
class PlaySpace;

class Weapon
{
public:
	Bullet BulletPrototype;	
	bool isShooting = false;
	float ShootTimer = 0.f;
public:
	Weapon();
	
	void update(float, PlaySpace*, Ship*);
};