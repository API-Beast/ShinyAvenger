//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Player.h"

void Player::draw(RenderContext r)
{
	r.Offset = Position;
	r.Rotation = MovementDirection.getAngle();
	Sprite.draw(r);
}

void Player::update(float t)
{
	Acceleration = MovementDirection * 250;
}