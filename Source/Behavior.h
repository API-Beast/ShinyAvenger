//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "PhysicsObject.h"

#include <Springbok/Animation/Interpolation.h>
#include <Springbok/Containers/List.h>
#include <Springbok/Platform/PreciseClock.h>

class Ship;
class PlaySpace;

// Behavior for AI
class Behavior
{
protected:
	PreciseClock Clock;
	Ship* findTargetFor(Ship*,PlaySpace*);
public:
	Behavior() { Clock.start(); }
	virtual void update(float t, Ship* const, PlaySpace*) = 0;
};

class EmptyBehavior : public Behavior { void update(float t, Ship *const ship, PlaySpace* space) { } };

class FollowingBehavior : public Behavior
{
private:
	Vec2F Waypoint;
public:
	void update(float t, Ship *const ship, PlaySpace* space);
};

class RotatingBehavior : public Behavior
{
public:
	void update(float t, Ship *const ship, PlaySpace* space);
};

class TrackingBehavior : public Behavior
{
private:
	Ship *Target;
public:
        TrackingBehavior(Ship *Target) : Target(Target) { }
	void update(float t, Ship *const ship, PlaySpace* space);
};