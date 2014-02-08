//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Enemy.h"
#include "EnemyBehavior.h"
#include <Springbok/Animation/Interpolation.h>
#include <iostream>
#include <cmath>

void Enemy::draw(RenderContext r)
{
	r.Offset = Position;
	r.Rotation = MovementDirection;
	Sprite.draw(r);
}

void Enemy::update(float t, PlaySpace *Space)
{	
	Acceleration = MovementDirection.toDirection() * 400 * AcclerateFactor;
	MovementDirection = Approach<Angle>(MovementDirection, TargetDirection, t);
	
	if(Abs(TargetDirection.difference(Speed.getAngle())) > 0.01) {
	  TargetDirection = Approach<Angle>(TargetDirection, Speed.getAngle(), (1-(AcclerateFactor*AcclerateFactor))*t);
	}
	AcclerateFactor = Approach(AcclerateFactor, 0.0f, t);
	
	if (Behavior != NULL) {
	    Behavior->update(t, this);
	}
}