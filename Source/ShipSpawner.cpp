//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "ShipSpawner.h"
#include "PlaySpace.h"
#include "Behavior.h"

void ShipSpawner::update(float delta, PlaySpace *space)
{
	if ((Time += delta) > Interval)
	{
		Time = Time - Interval;
		spawnGroup(space);
	}
}

void ShipSpawner::spawnGroup(PlaySpace *space)
{
	const int GROUP_SIZE = 2 + (space->GameTime / Interval) / 10.0f;
	
	Vec2F pos = RNG.generateVec2(-space->Size/2, space->Size/2);
	
	for(int i = 0; i < GROUP_SIZE; ++i)
	{
		spawnShip(space, pos + RNG.generate(-Vec2F(150, 150), Vec2F(150, 150)));
	}
}

void ShipSpawner::spawnShip(PlaySpace *space, Vec2F position)
{
	Ship* ship = new Ship(Prototype);
	ship->AI = new TrackingBehavior(space->Player);
	ship->Position = position;
	
	space->Ships.pushBack(ship);
	space->Objects.pushBack(ship);
}