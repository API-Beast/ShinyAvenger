//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Behavior.h"
#include "Ship.h"
#include "PlaySpace.h"

#include <iostream>

ShipAI::ShipAI(Leader* lead, Ship* ship, PlaySpace* space)
{
	Home = lead;
	NearbyShips = space->findShips({ship->Position - ViewRange}, {ship->Position + ViewRange});
}

void ShipAI::update(float t, Ship* ship, PlaySpace* space)
{
	FindTimer += t;
	if(FindTimer > 2.0f)
	{
		FindTimer = 0.f;
		NearbyShips = space->findShips({ship->Position - ViewRange}, {ship->Position + ViewRange});
		std::cout << "Found " << NearbyShips.length() << std::endl;
	}
	
	bool reachedTarget = false;
	switch(GoalState)
	{
		case Follow:
		{
			if(Target)
			{
				steerTowards(ship, Target->Position + Target->Speed*0.2f, 0.2f);
				if(InRange(ship->Position, Target->Position, 250))
					reachedTarget = true;
			}
			else
				reachedTarget = true;
			break;
		}
		case MoveTo:
		{
			steerTowards(ship, Waypoint);
			if(InRange(ship->Position, Waypoint, 250))
				reachedTarget = true;
			break;
		}
		case ShotAt:
		{
			if(Target)
			{
				steerTowards(ship, Target->Position + Target->Speed*0.2f, 0.2f, true);
				if(Target->isDestroyed())
					reachedTarget = true;
			}
			else
				reachedTarget = true;
			break;
		}
		case Wait:
			reachedTarget = true;
			break;
		default:
		{
			std::cout << "State broken" << std::endl;
			break;
		}
	}
	
	if(reachedTarget)
	{
		switch(TacticState)
		{
			case Orbit:
			{
				GoalState = MoveTo;
				Angle ang = Angle(ship->Position - Home->getPosition()) + 0.05_turn;
				Waypoint = Home->getPosition() + ang.toDirection() * OrbitDistance;
				break;
			}
			case Engage:
			{
				Target = findHostileTarget(ship, space, AggroRange);
				if(Target)
					GoalState = ShotAt;
				else
				{
					GoalState = Wait;
					TacticState = Orbit;
				}
				break;
			}
			default:
				break;
		}
	}
	else
	{
		if(TacticState == Orbit && GoalState != ShotAt)
		{
			Target = findHostileTarget(ship, space, AggroRange);
			if(Target)
				GoalState = ShotAt;
		}
	}
}

ObjectPointer< Ship > ShipAI::findHostileTarget(Ship* ship, PlaySpace* space, float range)
{
	for(Ship* other : NearbyShips)
		if(space->isHostile(ship, other) && InRange(ship->Position, other->Position, range) && !other->isDestroyed())
			return other;
	
	return nullptr;
}

void ShipAI::steerTowards(Ship* ship, Vec2F point, float timePrediction, bool fireAtPoint)
{
	Vec2F delta = (point - (ship->Position + ship->Speed * timePrediction));
	float distance = delta.length();
	
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
	
	if(Abs(angleDelta) > 0.25_turn || distance < 250)
		ship->IsBraking = true;
	else
		ship->IsBraking = false;
	
	if(fireAtPoint)
	{
		if(Abs(angleDelta) < 0.05_turn && distance < 750)
			ship->IsShooting = 1;
		else
			ship->IsShooting = false;
	}
}