//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "PhysicsObject.h"
#include <iostream>
#include <Springbok/Animation/Interpolation.h>

class Ship;
class PlaySpace;

class Behavior
{
public:
	virtual void update(float t, Ship* const, PlaySpace*) = 0;
};

class TrackingBehavior : public Behavior
{
public:
	Ship* Target;

	TrackingBehavior(Ship* t) : Target(t) {}
	void update(float t, Ship *const ship, PlaySpace* space);
};