//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.de>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

class PlaySpace;

class Sector
{
	
public:
	float Radius;	
	Vec2F Center;	
	int ID;	
public:
	Sector() { }
	
	void update(float, PlaySpace*);
};

