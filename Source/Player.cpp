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

Player::Player()
{
	BulletPrototype.Mass = 2;
	BulletPrototype.Drag = 5;
	BulletPrototype.Lifetime = 1.f;
	
	BulletPrototype.ColorAnimation[0.0].insert(Colors::White);
	BulletPrototype.ColorAnimation[0.5].insert(Colors::Saturated::Orange);
	BulletPrototype.ColorAnimation[1.0].insert(Colors::Saturated::Red);
	
	BulletPrototype.GlowColorAnimation[0.0].insert(Colors::Saturated::Yellow);
	BulletPrototype.GlowColorAnimation[0.5].insert(Colors::Saturated::Orange);
	BulletPrototype.GlowColorAnimation[1.0].insert(Colors::Saturated::Red);
	
	BulletPrototype.AlphaAnimation[0.0].insert(1.f);
	BulletPrototype.AlphaAnimation[0.8].insert(1.f);
	BulletPrototype.AlphaAnimation[1.0].insert(0.f);
	
	BulletPrototype.ScaleAnimation[0.0].insert(1.f);
	BulletPrototype.ScaleAnimation[0.2].insert(Vec2F(1.f, 4.f));
	BulletPrototype.ScaleAnimation[0.8].insert(Vec2F(1.5f, 3.f));
	BulletPrototype.ScaleAnimation[1.0].insert(Vec2F(0.5f, 4.f));
	
	BulletPrototype.GlowScaleAnimation[0.0].insert(2.f);
	BulletPrototype.GlowScaleAnimation[1.0].insert(2.f);
	
	BulletPrototype.Sprite = Image("Player/Bullet.png");
	BulletPrototype.Glow = Image("Glow.png");
}

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
			Bullet bullet(BulletPrototype);
			bullet.Position = Position;
			bullet.Speed = ShootingDirection.toDirection()*1000 + Speed *0.8;
			space->spawnPlayerBullet(bullet);
		}
	}
}