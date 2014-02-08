//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Player.h"
#include "Bullet.h"
#include "PlaySpace.h"
#include <Springbok/Animation/Interpolation.h>

#include <iostream>
#include <cmath>

void Player::draw(RenderContext r)
{
	r.Offset = Position;
	r.Rotation = MovementDirection;
	Sprite.draw(r);
}

void Player::update(float t, PlaySpace* space)
{	
	Acceleration = MovementDirection.toDirection() * 400 * AcclerateFactor;
	MovementDirection = Approach<Angle>(MovementDirection, TargetDirection, t);
	
	if(Abs(TargetDirection.difference(Speed.getAngle())) > 0.01) {
	  TargetDirection = Approach<Angle>(TargetDirection, Speed.getAngle(), (1-(AcclerateFactor*AcclerateFactor))*t);
	}
	AcclerateFactor = Approach(AcclerateFactor, 0.0f, t);
	
	if(IsShooting)
	{
		ShootTimer -= t;
		if(ShootTimer < 0)
		{
			ShootTimer = 0.1f;
			Bullet bullet;
			bullet.Sprite = Image("Player/Bullet.png");
			bullet.Position = Position;
			bullet.Speed = ShootingDirection.toDirection()*800 + Speed / 2;
			bullet.Mass = 3;
			bullet.Drag = 0;
			space->spawnPlayerBullet(bullet);
		}
	}
}