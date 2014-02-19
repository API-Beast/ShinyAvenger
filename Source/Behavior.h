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

// A state based behavior
class StateBasedBehavior : public Behavior
{
public:	
	List<Behavior*> States;
	virtual void update(float t, Ship *const ship, PlaySpace* space)
	{
		
	}
	
private:
	
};

class TrackingBehavior : public Behavior
{
public:
	Ship *Target = NULL;
	virtual void update(float t, Ship *const ship, PlaySpace* space);
private:
	Ship* findTargetFor(Ship*,PlaySpace*);
};