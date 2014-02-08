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
	r.Rotation = Acceleration.getAngle();
	Sprite.draw(r);
}

void Enemy::update(float t, PlaySpace *Space)
{	
	if (Behavior != NULL)
	{
		Behavior->update(t, this);
	}
}