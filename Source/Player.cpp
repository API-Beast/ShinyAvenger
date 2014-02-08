//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Player.h"
#include <Springbok/Animation/Interpolation.h>

#include <iostream>
#include <cmath>

void Player::draw(RenderContext r)
{
	r.Offset = Position;
	r.Rotation = MovementDirection;
	Sprite.draw(r);
}

void Player::update(float t)
{	
	Acceleration = MovementDirection.toDirection() * 2000 * AcclerateFactor;
	MovementDirection = Approach<Angle>(MovementDirection, TargetDirection, t);
	
	double diff = Speed.getAngle();
	float factor = 1;
	double tolerance = 0.01;
	
	if (diff < tolerance && diff > -tolerance) {
	  factor = 0;
	}
	
	TargetDirection = Approach<Angle>(TargetDirection, Speed.getAngle(), (factor-(AcclerateFactor*AcclerateFactor))*t);
	AcclerateFactor = Approach(AcclerateFactor, 0.0f, t);
}