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
: Prototype(Image("Player/Sprite.png"))
{
	Faction = faction;
	Radius = r;
	Prototype.Sprite = Image("Player/Sprite.png");
	Position = center;
	
	Color factionClr = space->getFactionColor(faction);
	
	TheGravitySource.Position = center;
	TheGravitySource.Range = r;
	TheGravitySource.Gravity = 100.f;
	TheGravitySource.BackgroundColor = ShiftHueRight(factionClr, 0.05f) * 0.75f;
	TheGravitySource.CenterColor = factionClr;
	TheGravitySource.HighlightColor = ShiftHueLeft(factionClr, 0.1f);
	
	space->GravitySources.pushBack(TheGravitySource);
}

void SolarSystem::update(float delta, PlaySpace *Space)
{
	if ((Time += delta) > Interval)
	{
		const int GROUP_SIZE = 4;
		Time = Time - Interval;
		
		float radius = RNG.generate(700.f, 1000.f);
		Angle angle = Angle::FromTurn(RNG.generate());
		
		spawnGroup(Position + angle.toDirection().normalized() * radius, GROUP_SIZE, Space);
	}
}

void SolarSystem::spawnGroup(Vec2F pos, int groupSize, PlaySpace *Space)
{
	
	const float SPRAY_FACTOR = 150.f;
	
	Ship* leader = NULL;
	
	for(int i = 0; i < groupSize; ++i)
	{
		Ship* ship = spawnShip(pos + RNG.generateVec2(-Vec2F(SPRAY_FACTOR, SPRAY_FACTOR), Vec2F(SPRAY_FACTOR, SPRAY_FACTOR)), Space);
	
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
	float radius = RNG.generate(700.f, 1000.f);
	Angle angle = Angle::FromTurn(RNG.generate());
	spawnShip(Position + angle.toDirection().normalized() * radius, space);
	
}

Ship* SolarSystem::spawnShip(Vec2F position, PlaySpace *Space)
{
	Ship* ship = new Ship(Prototype);
	ship->AI = new FollowingBehavior;
	ship->Position = position;
	ship->Faction = Faction;
	ship->Rotation = Angle::FromTurn(RNG.generate());
	ship->FactionColor = Space->getFactionColor(Faction);
	Space->Ships.pushBack(ship);
	Space->Objects.pushBack(ship);
	return ship;
}

Vec2F SolarSystem::getPosition() const
{
	return Position;
}