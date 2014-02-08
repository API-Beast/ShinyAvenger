//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Bullet.h"

void Bullet::update(float dt, PlaySpace* space)
{
	TimeSinceSpawn += dt;
}

void Bullet::draw(RenderContext r)
{
	r.Offset = Position;
	r.Rotation = Speed.getAngle();
	Sprite.draw(r);
}
