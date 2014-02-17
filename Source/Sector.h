//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.de>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <Springbok/Geometry/Vec2.h>
#include "Ship.h"

class PlaySpace;

class Sector
{
private:
	RandomNumberGenerator RNG;
	float Time;
	float Interval;
	
public:
	float Radius;	
	Vec2F Center;	
	int ID;		
	Ship Prototype;
public:
	Sector(Vec2F center, float r) : Center(center), Radius(r), Time(0), Interval(5.0), Prototype(Image("Null.png")) { }
	
        // Spawns a new group inside of the system (relative coordinates, 0|0 is center!
	void spawnGroup(Vec2F, PlaySpace*);
	
	void update(float, PlaySpace*);
	
private:
	void spawnShip(Vec2<float>, PlaySpace*);
};

