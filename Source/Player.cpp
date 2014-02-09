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
	BulletPrototype.Mass = 5;
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
	r.Rotation = Rotation;
	Sprite.draw(r);
}

void Player::update(float t, PlaySpace* space)
{	
	Acceleration = Rotation.toDirection() * 400;
	RotationSpeed += Steering * t;
	RotationSpeed = Approach(RotationSpeed, 0.f, t/2);
	Rotation += RotationSpeed * t;
	
	if(IsShooting)
	{
		ShootTimer -= t;
		if(ShootTimer < 0)
		{
			ShootTimer = 0.2f;
			Bullet bullet(BulletPrototype);
			bullet.Speed = Rotation.toDirection()*1000 + Speed *0.8;
			bullet.Position = Position + (Rotation+0.25).toDirection()*10;
			space->spawnPlayerBullet(bullet);
			bullet.Position = Position - (Rotation+0.25).toDirection()*10;
			space->spawnPlayerBullet(bullet);
		}
	}
}