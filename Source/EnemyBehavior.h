//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "PhysicsObject.h"
#include "Enemy.h"
#include <iostream>
#include <Springbok/Animation/Interpolation.h>

class EnemyBehavior
{
public:
	virtual void update(float t, Enemy* const) = 0;
};

class TrackingBehavior : public EnemyBehavior
{
public:
	PhysicsObject *Target;
	double Speed;

	TrackingBehavior(PhysicsObject *PhysicsTarget, const double s = 100.0) : Target(PhysicsTarget), Speed(s) {}

	void update(float t, Enemy *const TheEnemy)
	{
		Vec2F Distance = Target->Position - TheEnemy->Position;
		TheEnemy->TargetDirection = Approach<Angle, double>(TheEnemy->TargetDirection, Distance.getAngle(), t / 2.0f);		
		TheEnemy->Acceleration = TheEnemy->TargetDirection.toDirection() * Speed * t * 25.0;		
	}
};

class OrbitBehavior : public TrackingBehavior
{
public:
	PhysicsObject *Target;
	double Speed;
	
	OrbitBehavior(PhysicsObject *physicsTarget, const double s) : TrackingBehavior(physicsTarget, s) {}
};