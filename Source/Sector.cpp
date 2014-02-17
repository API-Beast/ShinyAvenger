//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.de>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Sector.h"
#include "PlaySpace.h"
#include "Behavior.h"

void Sector::update(float delta, PlaySpace *Space)
{
	if ((Time += delta) > Interval)
	{
		const int GROUP_SIZE = 2 + (Space->GameTime / 5000.f) / 10.0f;
		Time = Time - Interval;
		
		float radius = RNG.generate(50.f, Radius);
		Angle angle = RNG.generate(0_turn, 1_turn);
		
		spawnGroup(angle.toDirection() * radius, GROUP_SIZE, Space);
	}
}

void Sector::spawnGroup(Vec2F pos, int groupSize, PlaySpace *Space)
{
	
	const float SPRAY_FACTOR = 150.f;
	
	
	for(int i = 0; i < groupSize; ++i)
	{
		spawnShip(pos + RNG.generateVec2<>(-Vec2F(SPRAY_FACTOR, SPRAY_FACTOR), Vec2F(SPRAY_FACTOR, SPRAY_FACTOR)), Space);
	}
}

void Sector::spawnShip(Vec2F position, PlaySpace *Space)
{
	Ship* ship = new Ship(Prototype);
	ship->AI = new TrackingBehavior(Space->Player);
	ship->Position = position;
	ship->FactionColor = Space->getFactionColor(rand() % 1000);
	Space->Ships.pushBack(ship);
	Space->Objects.pushBack(ship);
}