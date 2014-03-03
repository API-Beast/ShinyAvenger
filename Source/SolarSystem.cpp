//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.de>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "SolarSystem.h"
#include "PlaySpace.h"
#include "Behavior.h"

static RandomNumberGenerator RNG;

SolarSystem::SolarSystem(Vec2F center, float r, PlaySpace* space, int faction) 
: Prototype(Image("Enemy/Enemy01.png"))
{
	Faction = faction;
	Radius = r;
	Position = center;
	Prototype.FactionColorSprite = Image("Enemy/Enemy01FactionColor.png");
	Prototype.ImpulseColor = Palette::Lilac;
	
	Color factionClr = space->getFactionColor(faction);
	
	TheGravitySource.Position = center;
	TheGravitySource.Range = r;
	TheGravitySource.CenterSize = r * 0.0004f * (0.60f + RNG.getFloat() * 0.8f);
	TheGravitySource.Gravity = 100.f;
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
		
		float radius = RNG.getNumber(700.f, 1000.f);
		Angle angle = Angle::FromTurn(RNG.getFloat());
		
		spawnGroup(Position + angle.toDirection().normalized() * radius, GROUP_SIZE, Space);
	}
}

void SolarSystem::spawnGroup(Vec2F pos, int groupSize, PlaySpace *Space)
{
	
	const float SPRAY_FACTOR = 150.f;
	
	Ship* leader = NULL;
	
	for(int i = 0; i < groupSize; ++i)
	{
		Ship* ship = spawnShip(pos + RNG.getVec2(-Vec2F(SPRAY_FACTOR, SPRAY_FACTOR), Vec2F(SPRAY_FACTOR, SPRAY_FACTOR)), Space);
	
		if (leader != NULL)
		{
			ship->TheLeader = leader;
		}
		else
		{
			ship->TheLeader = this;
			leader = ship;
		}
	}
}

Ship* SolarSystem::spawnShip(PlaySpace* space)
{
	float radius = RNG.getNumber(700.f, 1000.f);
	Angle angle = Angle::FromTurn(RNG.getFloat());
	spawnShip(Position + angle.toDirection().normalized() * radius, space);
	
}

Ship* SolarSystem::spawnShip(Vec2F position, PlaySpace *Space)
{
	Ship* ship = new Ship(Prototype);
	ship->AI = new ShipAI(this, ship, Space);
	ship->Position = position;
	ship->Faction = Faction;
	ship->Rotation = Angle::FromTurn(RNG.getFloat());
	ship->FactionColor = Space->getFactionColor(Faction);
	Space->Ships.pushBack(ship);
	Space->Objects.pushBack(ship);
	return ship;
}

Vec2F SolarSystem::getPosition() const
{
	return Position;
}