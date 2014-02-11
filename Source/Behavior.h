//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "PhysicsObject.h"
#include <iostream>
#include <Springbok/Animation/Interpolation.h>
#include "Ship.h"

class Ship;

class Behavior
{
public:
	virtual void update(float t, Ship* const) = 0;
};

class StupidTrackingBehavior : public Behavior
{
public:
	PhysicsObject *Target;
	double Speed;

	StupidTrackingBehavior(PhysicsObject *PhysicsTarget, const double s = 100.0) : Target(PhysicsTarget), Speed(s) {}

	void update(float t, Ship *const TheShip)
	{
		Vec2F Distance = Target->Position - TheShip->Position;
		if (Distance.getLength() < 1000.0) {
			TheShip->Rotation = Approach<Angle, float>(TheShip->Rotation, Distance.getAngle(), t / 2.0f);		
			TheShip->Acceleration = TheShip->Rotation.toDirection() * Speed;
		}		
	}
};