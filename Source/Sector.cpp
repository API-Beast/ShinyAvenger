//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.de>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Sector.h"
#include "PlaySpace.h"
#include "Behavior.h"


static RandomNumberGenerator RNG;

Sector::Sector(Vec2F center, float r, PlaySpace *Space) 
: Radius(r), Time(0), Interval(8.0), Prototype(Image("Player/Sprite.png")),
TheGravitySource({center, 250.f, r * 2.f, ColorRGB(0.62f, 0.2f, 0.44f), ColorRGB(0.92f, 0.5f, 0.44f)})
{
	Position = center;	
	ID = RNG.generate() * 30000.0f;
	
	TheGravitySource.BackgroundColor = Space->getFactionColor(ID) * 0.2f;
	TheGravitySource.CenterColor = Space->getFactionColor(ID);
	
	Space->GravitySources.pushBack(TheGravitySource);
}

void Sector::update(float delta, PlaySpace *Space)
{
	if ((Time += delta) > Interval)
	{
		const int GROUP_SIZE = 4;
		Time = Time - Interval;
		
		float radius = RNG.generate(200.f, Radius);
		Angle angle = Angle::FromTurn(RNG.generate());
		
		spawnGroup(Position + angle.toDirection().normalized() * radius, GROUP_SIZE, Space);
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

Ship* Sector::spawnShip(Vec2F position, PlaySpace *Space)
{
	Ship* ship = new Ship(Prototype);
	ship->AI = new TrackingBehavior(NULL);
	ship->Position = position;
	ship->Faction = ID;
	ship->Rotation = Angle::FromTurn(RNG.generate());
	ship->FactionColor = Space->getFactionColor(ID);
	Space->Ships.pushBack(ship);
	Space->Objects.pushBack(ship);
	return ship;
}

Vec2F Sector::getPosition() const
{
	return Position;
}