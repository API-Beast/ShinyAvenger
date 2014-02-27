//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Behavior.h"
#include "Ship.h"
#include "PlaySpace.h"

#include <iostream>

static RandomNumberGenerator RNG;

void FollowingBehavior::update(float t, Ship* const ship, PlaySpace* space)
{
	if (ship->TheLeader != nullptr)
	{		
		Vec2F LeaderPos = ship->TheLeader->getPosition();
		
		// Check for movement
		if (!ship->TheLeader->hasMoved())
		{
			delete ship->AI;
			ship->AI = new RotatingBehavior;
			return;
		}
		
		if (Waypoint.length() < 0.1f)
		{
			Waypoint = LeaderPos;
		}
		
		Vec2F delta = ((Waypoint) - (ship->Position));
		float distance = delta.length();
		
		if(distance < 250)
		{
			Waypoint = LeaderPos;
			delta = ((Waypoint) - (ship->Position));
			distance = delta.length();
		}
		
		Angle targetRotation = Angle(delta);
		Angle angleDelta = ship->Rotation - targetRotation;
		std::cout << Waypoint << std::endl;
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
	}
}

void RotatingBehavior::update(float t, Ship* const ship, PlaySpace* space)
{
	if (ship->TheLeader != nullptr)
	{
		// Check for movement
		if (ship->TheLeader->hasMoved())
		{
			delete ship->AI;
			ship->AI = new FollowingBehavior;
			return;
		}
		
		ship->Rotation = Angle(ship->TheLeader->getPosition() - ship->getPosition()) + 0.1_turn;		
		
		if (Clock.elapsed() > 0.5)
		{
			Clock.start();
			
			Ship* newTarget = findTargetFor(ship, space);
			
			if (newTarget != nullptr)
			{
			    delete ship->AI;
			    ship->AI = new TrackingBehavior(newTarget);
			}
		}
	}
}

Ship* Behavior::findTargetFor(Ship *ship, PlaySpace *Space)
{
	RandomNumberGenerator RNG;	
	auto nearShips = Space->findShips({ship->Position - ship->AggroRadius}, {ship->Position + ship->AggroRadius});

	for(Ship* other : nearShips)
		if(RNG.getFloat() < 0.2f && Space->isHostile(ship, other) && other->Status != Ship::ShipState::Destroyed)
			return other;
	
	return NULL;
}


void TrackingBehavior::update(float t, Ship* const ship, PlaySpace* space)
{
	if (Target != NULL && Target->Status != Ship::ShipState::Destroyed && space->isHostile(Target, ship))
	{
		Vec2F delta = ((Target->Position + Target->Speed * 0.2f) - (ship->Position + ship->Speed * 0.2f));
		float distance = delta.length();
		if(distance < 250)
		{
			delta = ((Target->Position + Target->Speed * 0.1f) - (ship->Position + ship->Speed * 0.1f));
			distance = delta.length();
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
		delete ship->AI;
		ship->AI = new FollowingBehavior;
	}
}