//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "PhysicsObject.h"

#include <Springbok/Animation/Interpolation.h>
#include <Springbok/Containers/List.h>

class Ship;
class PlaySpace;

// Behavior for AI
class Behavior
{
public:
	virtual void update(float t, Ship* const, PlaySpace*) = 0;
};

class EmptyBehavior : public Behavior { void update(float t, Ship *const ship, PlaySpace* space) { } };

class FollowingBehavior : public Behavior
{
public:
	void update(float t, Ship *const ship, PlaySpace* space);
};

class RotatingBehavior : public Behavior
{
private:
	
	float radius = 0;
public:
	void update(float t, Ship *const ship, PlaySpace* space);
};

class TrackingBehavior : public Behavior
{
public:
	Ship *Target = nullptr;
	void update(float t, Ship *const ship, PlaySpace* space);
private:
	Ship* findTargetFor(Ship*,PlaySpace*);
};