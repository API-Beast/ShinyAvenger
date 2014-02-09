//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

class PlaySpace;

class EnemySpawner
{
private:
	float Time;
	float Interval;
public:
	EnemySpawner() : Time(0), Interval(5.0) { }
	
	void update(float, PlaySpace*);
};