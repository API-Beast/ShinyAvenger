//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Behavior.h"
#include "Ship.h"

void TrackingBehavior::update(float t, Ship* const ship, PlaySpace* space)
{
	Vec2F delta = ((Target->Position) - ship->Position);
	float distance = delta.getLength();
	Vec2F predictedDelta = ((Target->Position) + Target->Speed - ship->Position + ship->Speed) / (distance / ship->Speed.getLength());
	
	Angle targetRotation = predictedDelta.getAngle();
	Angle angleDelta = targetRotation - ship->Rotation;
	
	// TODO: Something is really weird here. More bugs in the Angle class?
	if(Abs(angleDelta) > 0.01f && Abs(Angle(Target->RotationSpeed) - angleDelta) > t)
	{
		if(angleDelta > 0)
			ship->Steering = -1;
		else
			ship->Steering =  1;
	}
	else
		ship->Steering = 0;
	
	if(Abs(angleDelta) < 0.01f && distance < 750)
		ship->IsShooting = 1;
	else
		ship->IsShooting = false;
	
	if(Abs(targetRotation - ship->Speed.getAngle()) < 0.01f || angleDelta > 0.25f)
		ship->IsBraking = true;
	else
		ship->IsBraking = false;
}
