//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.de>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Sector.h"
#include "PlaySpace.h"
#include "Behavior.h"

Sector::Sector(Vec2F center, float r, PlaySpace *Space) 
: Center(center), Radius(r), Time(0), Interval(5.0), Prototype(Image("Null.png")),
  TheGravitySource({center, 100.f, r, ColorRGB(0.62f, 0.2f, 0.44f), ColorRGB(0.92f, 0.5f, 0.44f)})
{ 
	ID = RNG.generate() * sizeof(int);
	
	Prototype.Sprite = Image("Player/Sprite.png");
	Prototype.PrimaryWeapon.BulletPrototype.Power = 2.f;
	Prototype.PrimaryWeapon.ShotDelay = 0.4f;
	Prototype.EngineAccleration *= 1.3f;
	
	TheGravitySource.BackgroundColor = Space->getFactionColor(ID) * 0.2f;
	TheGravitySource.CenterColor = Space->getFactionColor(ID);
	Space->GravitySources.pushBack(TheGravitySource);
}
void Sector::update(float delta, PlaySpace *Space)
{
	if ((Time += delta) > Interval)
	{
		const int GROUP_SIZE = 2 + (Space->GameTime / 5000.f) / 10.0f;
		Time = Time - Interval;
		
		float radius = RNG.generate(50.f, Radius);
		Angle angle = Angle::FromTurn(RNG.generate());
		
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