//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Behavior.h"
#include "Ship.h"

#include <iostream>

void TrackingBehavior::update(float t, Ship* const ship, PlaySpace* space)
{
	Vec2F delta = ((Target->Position + Target->Speed * 0.1f) - (ship->Position + Target->Position * 0.1));
	float distance = delta.getLength();
	
	Angle targetRotation = Angle(delta);
	Angle angleDelta = ship->Rotation - targetRotation;
	
	if(Abs(angleDelta) > 0.1_turn)
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
	
	if(Abs(angleDelta) > 0.25_turn)
		ship->IsBraking = true;
	else
		ship->IsBraking = false;
	
	//std::cout << ship->Rotation << " -> " << targetRotation << " = " << angleDelta << " (" << ship->IsShooting << ")" << std::endl;
}
