//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.de>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "SolarSystem.h"
#include "PlaySpace.h"
#include "Behavior.h"
#include "AssetDefinition.h"

SolarSystem::SolarSystem(Vec2F center, float r, PlaySpace* space, int faction) 
: Prototype(gAssets.Fighter)
{
	Faction = faction;
	Radius = r;
	Position = center;
	
	Color factionClr = space->getFactionColor(faction);
	
	TheGravitySource.Position = center;
	TheGravitySource.Range = r;
	TheGravitySource.CenterSize = r * 0.0004f * (0.60f + gRNG.getFloat() * 0.8f);
	TheGravitySource.Gravity = 25.f;
	TheGravitySource.BackgroundColor = ShiftHueRight(factionClr, 0.05f) * 0.75f;
	TheGravitySource.CenterColor = factionClr;
	TheGravitySource.HighlightColor = ShiftHueLeft(factionClr, 0.1f);
	
	space->GravitySources.pushBack(TheGravitySource);
	Clock.start();
}

void SolarSystem::update(float delta, PlaySpace *Space)
{
	if (Clock.elapsed() > Interval)
	{
		const int GROUP_SIZE = 4;
		Clock.start();
		
		float radius = gRNG.getNumber(100+TheGravitySource.CenterSize*800, TheGravitySource.CenterSize*1200.f);
		Angle angle = Angle::FromTurn(gRNG.getFloat());
		
		spawnGroup(Position + angle.toDirection().normalized() * radius, GROUP_SIZE, Space);
	}
}

void SolarSystem::spawnGroup(Vec2F pos, int groupSize, PlaySpace *Space)
{
	const float SPRAY_FACTOR = 150.f;
	
	Ship* leader = NULL;
	
	for(int i = 0; i < groupSize; ++i)
	{
		Ship* ship = spawnShip(pos + gRNG.getVec2(-Vec2F(SPRAY_FACTOR, SPRAY_FACTOR), Vec2F(SPRAY_FACTOR, SPRAY_FACTOR)), Space);
	}
}

Ship* SolarSystem::spawnShip(PlaySpace* space)
{
	spawnShip(generatePosition(), space);
}

Vec2F SolarSystem::generatePosition() const
{
	float radius = gRNG.getNumber(100+TheGravitySource.CenterSize*800, TheGravitySource.CenterSize*1200.f);
	Angle angle = Angle::FromTurn(gRNG.getFloat());
	return Position + angle.toDirection().normalized() * radius;
}

Ship* SolarSystem::spawnShip(Vec2F position, PlaySpace *Space)
{
	Ship* ship = new Ship(Prototype);
	ship->AI = new ShipAI(this, ship, Space);
	ship->Position = position;
	ship->Faction = Faction;
	ship->Rotation = Angle::FromTurn(gRNG.getFloat());
	ship->FactionColor = Space->getFactionColor(Faction);
	Space->Ships.pushBack(ship);
	Space->Objects.pushBack(ship);
	return ship;
}

Vec2F SolarSystem::getPosition() const
{
	return Position;
}