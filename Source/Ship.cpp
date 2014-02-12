//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Ship.h"
#include "Behavior.h"
#include "PlaySpace.h"

Ship::Ship()
{
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

void Ship::update(float t, PlaySpace* space)
{	
	if(AI)
		AI->update(t, this, space);
	
	updateControls(t, space);
	updateWeapon  (t, space);
	updateFX      (t, space);
}

void Ship::updateControls(float t, PlaySpace* space)
{
	if(IsBraking)
		Acceleration = 0;
	else
		Acceleration = Rotation.toDirection() * EngineAccleration;
	
	RotationSpeed += Steering * t * 0.5f;
	Flow = 1-Abs(Steering);
	Stabilizer = IsStabilizing;
}

void Ship::updateFX(float t, PlaySpace* space)
{
	ImpulseFXTimer -= t;
	if(IsBraking)
		return;
	if(ImpulseFXTimer > 0)
		return;
	
	ImpulseFXTimer = 0.05f;
	// Spawn glow
	Particle glow(ImpulseParticle);
	glow.Rotation = Rotation;
	glow.Position = Position + (Rotation+0.5f).toDirection()*24;
	glow.Speed = -Acceleration + Speed;
	space->spawnParticle(glow);
	// Spawn sparks
	for(int i = 0; i < 10; ++i)
	{
		Particle spark(SparkParticle);
		spark.Rotation = Rotation;
		spark.Position = Position + (Rotation+0.5f).toDirection()*(14+RNG.generate()*10) + (Rotation+0.25f).toDirection() * RNG.generate(-4.f, 4.f);
		spark.Speed = -Acceleration + Speed + (Rotation+0.25f).toDirection() * RNG.generate(-60.f, 60.f);
		space->spawnParticle(spark);
	}
}

void Ship::updateWeapon(float t, PlaySpace* space)
{
	Weapon.ShotTimer -= t;
	if(IsShooting)
	{
		if(Weapon.ShotTimer <= 0.f)
		{
			Weapon.ShotTimer = Weapon.ShotDelay;
			
			Bullet bullet(Weapon.BulletPrototype);
			bullet.Rotation = Rotation;
			bullet.Speed = Rotation.toDirection()*1000 + Speed *0.8;
			bullet.Position = Position + (Rotation+0.25f).toDirection()*10;
			space->spawnPlayerBullet(bullet);
			bullet.Position = Position - (Rotation+0.25f).toDirection()*10;
			space->spawnPlayerBullet(bullet);
		}
	}
}

void Ship::draw(RenderContext r)
{
	r.setColor(FractionColor, 1.f);
	r.Offset = Position;
	r.Rotation = Rotation;
	Sprite.draw(r);
}

Ship::~Ship()
{
	//delete AI;
}
