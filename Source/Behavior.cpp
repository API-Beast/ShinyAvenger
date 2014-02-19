//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Behavior.h"
#include "Ship.h"
#include "PlaySpace.h"

#include <iostream>

void LeadBehavior::update(float t, Ship* const ship, PlaySpace* space)
{
	
}

void TrackingBehavior::update(float t, Ship* const ship, PlaySpace* space)
{
	if (Target != NULL && Target->Status != Ship::ShipState::Destroyed && space->isHostile(Target, ship))
	{
		Vec2F delta = ((Target->Position + Target->Speed * 0.2f) - (ship->Position + ship->Speed * 0.2f));
		float distance = delta.getLength();
		if(distance < 250)
		{
			delta = ((Target->Position + Target->Speed * 0.1f) - (ship->Position + ship->Speed * 0.1f));
			distance = delta.getLength();
		}
		
		Angle targetRotation = Angle(delta);
		Angle angleDelta = ship->Rotation - targetRotation;
		
		if(Abs(angleDelta) > 0.01_turn)
		{
			ship->IsStabilizing = 0;
			if(angleDelta > 0_turn)
				ship->Steering = -1;
			else
				ship->Steering =  1;
		}
		else
		{
			ship->Steering = 0;
			ship->IsStabilizing = 1;
		}
		
		if(Abs(angleDelta) < 0.05_turn && distance < 750)
			ship->IsShooting = 1;
		else
			ship->IsShooting = false;
		
		if(Abs(angleDelta) > 0.25_turn || distance < 500)
			ship->IsBraking = true;
		else
			ship->IsBraking = false;
	} 
	else 
	{
		// Fly away!
		ship->Steering = 0;
		ship->IsBraking = false;
		ship->IsShooting = false;
		ship->IsStabilizing = 1;
		
		Target = findTargetFor(ship, space);
	}
}

Ship* TrackingBehavior::findTargetFor(Ship *ship, PlaySpace *Space)
{
	for (Ship* other : Space->Ships)
	{
	    if (Space->isHostile(ship, other) && other->Status != Ship::ShipState::Destroyed)
	    {
		return other;    
	    }
	}
	
	return NULL;
}
