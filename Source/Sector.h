//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.de>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <Springbok/Geometry/Vec2.h>
#include "Ship.h"
#include "GravitySource.h"
#include "Leader.h"

class PlaySpace;

class Sector : public Leader
{
private:
	float Time = 0.0f;
	float Interval = 16.f;
public:
	float Radius = 100;	
	int ID = -1;		
	Ship Prototype;
	GravitySource TheGravitySource;
	Vec2F Position = 0;
public:
	Sector(Vec2F center, float r, PlaySpace*);
        // Spawns a new group inside of the system (relative coordinates, 0|0 is center!
	void spawnGroup(Vec2F, int, PlaySpace*);
	Ship* spawnShip(Vec2<float>, PlaySpace*);
	void update(float, PlaySpace*);
	virtual Vec2F getPosition() const;
};

