//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "EnemySpawner.h"
#include "PlaySpace.h"

#include <iostream>

void EnemySpawner::update(float delta, PlaySpace *Space)
{	
	TotalTime += delta;
	Time += delta;
	
	if (Time > Interval)
	{
		Time = Time - Interval;
		std::cout << "Spawn enemies!" << std::endl;
	}
}