//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "PhysicsObject.h"
#include "PlaySpace.h"

#include <Springbok/Animation/Interpolation.h>
#include <Springbok/Containers/List.h>
#include <Springbok/Platform/PreciseClock.h>

class Ship;
class PlaySpace;

// Behavior for AI
class Behavior
{
public:
	virtual void update(float t, Ship*, PlaySpace*) = 0;
	virtual void onHit(Ship* by, Ship* ship){};
};

class ShipAI : public Behavior
{
public:
	ShipAI(Leader* lead, Ship* ship, PlaySpace* space);
	virtual void update(float t, Ship* ship, PlaySpace* space);
	ObjectPointer<Ship> findHostileTarget(Ship* ship, PlaySpace* space, float range);
	void steerTowards(Ship* ship, Vec2F point, float timePrediction = 0.f, bool fireAtPoint = false);
public:
	enum _TacticState
	{
		Orbit,
		Engage
	} TacticState = Orbit;
	
	enum _GoalState
	{
		Wait,
		Follow,
		MoveTo,
		ShotAt
	} GoalState = Wait;
	
	Vec2F Waypoint;
	ObjectPointer<Ship> Target;
	Leader* Home;
	float FindTimer;
	decltype(PlaySpace().findShips(Vec2F(), Vec2F())) NearbyShips;
	
	float ViewRange  = 600.f;
	float AggroRange = 250.f;
	float OrbitRange = 600.f;
	
	float OrbitDistance  = 1500.f;
	float FollowDistance = 250.f;
};