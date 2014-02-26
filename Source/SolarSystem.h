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

class SolarSystem : public Leader
{
private:
	float Time = 0.0f;
	float Interval = 16.f;
public:
	float Radius = 100;	
	int Faction = -1;
	Ship Prototype;
	GravitySource TheGravitySource;
	Vec2F Position = 0;
public:
	SolarSystem(Vec2F center, float r, PlaySpace*, int);
        // Spawns a new group inside of the system (relative coordinates, 0|0 is center!
	void spawnGroup(Vec2F, int, PlaySpace*);
	Ship* spawnShip(Vec2<float>, PlaySpace*);
	Ship* spawnShip(PlaySpace* space);
	void update(float, PlaySpace*);
	Vec2F getPosition() const;
	bool hasMoved() const { return false; }
};

