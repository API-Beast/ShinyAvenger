//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Ship.h"

class PlaySpace;

class ShipSpawner
{
public:
	float Time;
	float Interval;
	Ship Prototype;
	RandomNumberGenerator RNG;
public:
	ShipSpawner() : Time(0), Interval(5.0) { }
	
	void update(float delta, PlaySpace* space);
	void spawnGroup(PlaySpace*);
	void spawnShip(PlaySpace*, Vec2F pos);
};