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
	
	ImpulseParticle.Sprite = Image("Player/Impulse.png");
	ImpulseParticle.DrawMode = RenderContext::Additive;
	ImpulseParticle.Lifetime = 0.2f;
	ImpulseParticle.Drag = 10;
	ImpulseParticle.Mass = 0.5f;
	
	ImpulseParticle.Animation.Alpha[0.0].insert(1.0f);
	ImpulseParticle.Animation.Alpha[1.0].insert(0.0f);
	
	ImpulseParticle.Animation.Color[0.0].insert(Colors::Saturated::Orange);
	ImpulseParticle.Animation.Color[1.0].insert(Colors::Saturated::Magenta);
	
	ImpulseParticle.Animation.Scale[0.0].insert(Vec2F(1.0f, 1.0f));
	ImpulseParticle.Animation.Scale[0.3].insert(Vec2F(1.0f, 2.0f));
	ImpulseParticle.Animation.Scale[1.0].insert(Vec2F(1.0f, 3.0f));
	
	ImpulseParticle.Animation.Rotation[0.0].insert(0.0f);
	ImpulseParticle.Animation.Rotation[1.0].insert(0.0f);
	
	SparkParticle = ImpulseParticle;
	SparkParticle.Sprite = Image("Player/Spark.png");
	SparkParticle.Drag = 0;
	SparkParticle.Mass = 5;
	SparkParticle.Lifetime = 0.15f;
	
	SparkParticle.Animation.Alpha = KeyframeList<float>();
	SparkParticle.Animation.Alpha[0.0].insert(0.0f);
	SparkParticle.Animation.Alpha[0.2].insert(1.0f);
	SparkParticle.Animation.Alpha[1.0].insert(0.0f);
}

void Player::draw(RenderContext r)
{
	r.Offset = Position;
	r.Rotation = Rotation;
	Sprite.draw(r);
}

void Player::update(float t, PlaySpace* space)
{	
	if(Braking)
		Acceleration = 0;
	else
	{
		Acceleration = Rotation.toDirection() * 400;
		ImpulseTimer -= t;
		if(ImpulseTimer < 0)
		{
			ImpulseTimer = 0.05f;
			Particle glow(ImpulseParticle);
			glow.Rotation = Rotation;
			glow.Position = Position + (Rotation+0.5).toDirection()*24;
			glow.Speed = -Acceleration + Speed;
			
			space->spawnParticle(glow);
			
			for (int i = 0; i < 10; ++i)
			{
				Particle spark(SparkParticle);
				spark.Rotation = Rotation;
				spark.Position = Position + (Rotation+0.5).toDirection()*(14+RNG.generate()*10) + (Rotation+0.25).toDirection() * RNG.generate(-4.f, 4.f);
				spark.Speed = -Acceleration + Speed + (Rotation+0.25).toDirection() * RNG.generate(-60.f, 60.f);
				space->spawnParticle(spark);
			}
		}
	}
	
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
			bullet.Position = Position + (Rotation+0.25f).toDirection()*10;
			space->spawnPlayerBullet(bullet);
			bullet.Position = Position - (Rotation+0.25f).toDirection()*10;
			space->spawnPlayerBullet(bullet);
		}
	}
}