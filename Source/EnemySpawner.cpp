//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "EnemySpawner.h"
#include "PlaySpace.h"

void spawnGroup(PlaySpace*, const float&);
void spawnEnemy(PlaySpace*, const float&, const float&);

void EnemySpawner::update(float delta, PlaySpace *Space)
{
	if ((Time += delta) > Interval)
	{
		Time = Time - Interval;
		spawnGroup(Space, Interval);
	}
}

void spawnGroup(PlaySpace *Space, const float &Interval)
{
	const int GROUP_SIZE = 2 + (Space->GameTime / Interval) / 10.0f;
	
	float x = rand() % (Space->Size.X) - Space->Size.X / 2.0f;
	float y = rand() % (Space->Size.Y) - Space->Size.Y / 2.0f;
	
	for (int i = 0; i < GROUP_SIZE; ++i)
	{
		spawnEnemy(Space, x, y);
	}
}

void spawnEnemy(PlaySpace *Space, const float &groupX, const float &groupY)
{
	const int SPRAY_FACTOR = 300.0;
	
	float x = groupX + rand() % SPRAY_FACTOR - SPRAY_FACTOR / 2.0f;
	float y = groupY + rand() % SPRAY_FACTOR - SPRAY_FACTOR / 2.0f;
	
	const double ShipSpeed = rand() % 50 + 100.0;
	EnemyBehavior *behavior = new TrackingBehavior(Space->ThePlayer, ShipSpeed);
	Enemy *TheEnemy = new Enemy(behavior);
	TheEnemy->Position.X = x;
	TheEnemy->Position.Y = y;
	
	Space->Enemies.pushBack(TheEnemy);
	Space->Objects.pushBack(TheEnemy);
}