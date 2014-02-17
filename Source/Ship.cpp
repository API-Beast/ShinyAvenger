//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Ship.h"
#include "Behavior.h"
#include "PlaySpace.h"

Ship::Ship(const Image& img) : Sprite(img)
{	
	FactionColorSprite = Image("Player/FactionColor.png");
	
	ImpulseParticle.Sprite = Image("Player/Impulse.png");
	ImpulseParticle.DrawMode = RenderContext::Additive;
	ImpulseParticle.Lifetime = 0.2f;
	ImpulseParticle.Drag = 10;
	ImpulseParticle.Mass = 0.5f;
	
	{
		auto& anim = ImpulseParticle.Animation.Alpha;
		anim.insert(1.0f, 0.0f);
	}
	
	{
		auto& anim = ImpulseParticle.Animation.Color;
		anim.clear();
		anim.insert(0.0f, Colors::Saturated::Orange);
		anim.insert(1.0f, Colors::Saturated::Magenta);
	}
	
	{
		auto& anim = ImpulseParticle.Animation.Scale;
		anim.clear();
		anim.insert(0.0f, Vec2F(1.0f, 1.0f));
		anim.insert(0.3f, Vec2F(1.0f, 2.0f));
		anim.insert(1.0f, Vec2F(1.0f, 3.0f));
	}
	
	SparkParticle = ImpulseParticle;
	SparkParticle.Sprite = Image("Player/Spark.png");
	SparkParticle.Drag = 0;
	SparkParticle.Mass = 5;
	SparkParticle.Lifetime = 0.15f;
	
	{
		auto& anim = SparkParticle.Animation.Alpha;
		anim.clear();
		anim.insert(0.0f, 0.0f);
		anim.insert(0.2f, 1.0f);
		anim.insert(1.0f, 0.0f);
	}
	
	ShieldParticle.Sprite = Image("Ship/Shield.png");
	ShieldParticle.DrawMode = RenderContext::Additive;
	ShieldParticle.Lifetime = 0.2f;
	ShieldParticle.Drag = 0.f;
	
	{
		auto& anim = ShieldParticle.Animation.Alpha;
		anim.clear();
		anim.insert(0.0f, 1.0f);
		anim.insert(0.5f, 0.2f);
		anim.insert(1.0f, 0.0f);
	}
	
	ShieldParticle.Animation.Color.insert(1.0f, Color(0.0f, 0.0f, 0.4f));
	ShieldParticle.Animation.Scale = Vec2F(0.75f, 0.75f);
	
	ShieldColors.insert(0.0f, Colors::White);
	ShieldColors.insert(0.5f, Colors::Saturated::Cyan);
	ShieldColors.insert(1.0f, Colors::Saturated::Red);
}

void Ship::update(float t, PlaySpace* space)
{	
	if(AI)
		AI->update(t, this, space);
	
	if(Status != Destroyed)
	{
		updateControls(t, space);
		updateWeapon  (t, space);
		updateFX      (t, space);
	}
	else
		Acceleration = 0;
}

void Ship::updateControls(float t, PlaySpace* space)
{
	if(IsBraking)
		Acceleration = 0;
	else
		Acceleration = Rotation.toDirection() * EngineAccleration;
	
	RotationSpeed += Steering * 4 * t;
	Flow = 1;
	Stabilizer = IsStabilizing;
}

void Ship::updateFX(float t, PlaySpace* space)
{
	if(IsBraking)
		return;
	ImpulseTimer -= t;
	ImpulseSparkTimer -= t;
	
	while(ImpulseTimer < 0)
	{
		// Spawn glow
		float supplementT = -ImpulseTimer;
		if(supplementT < 0)
			supplementT = 0;
		
		Particle glow(ImpulseParticle);
		glow.Rotation = Rotation;
		glow.Position = Position - Rotation.toDirection()*24;
		glow.Speed = -Acceleration + Speed;
		
		glow.update(supplementT, space);
		space->applyPhysics(&glow, supplementT);
		space->spawnParticle(glow);
		ImpulseTimer += 0.050f;
	}
	
	while(ImpulseSparkTimer < 0)
	{
		// Spawn sparks
		float supplementT = -ImpulseSparkTimer;
		if(supplementT < 0)
			supplementT = 0;
		
		Particle spark(SparkParticle);
		spark.Rotation = Rotation;
		spark.Position = Position - Rotation.toDirection()*(16+RNG.generate()*8) + (Rotation+0.25_turn).toDirection() * RNG.generate(-4.f, 4.f);
		spark.Speed = -Acceleration + Speed + (Rotation+0.25_turn).toDirection() * RNG.generate(-60.f, 60.f);
		spark.Lifetime = 0.15f + 0.10f * (RNG.generate()-0.5);
		
		spark.update(supplementT, space);
		space->applyPhysics(&spark, supplementT);
		space->spawnParticle(spark);
		ImpulseSparkTimer += 0.005f;
	}
}

void Ship::shootBullet(PlaySpace* space, Bullet prototype, float deltaTimeFix, int xOffset, Angle rotation)
{
	Bullet bullet(prototype);
	bullet.Faction = Faction;
	bullet.Rotation = Rotation + rotation;
	bullet.Speed = (Rotation + rotation ).toDirection()*1000 + Speed *0.8;
	bullet.Position = Position + (Rotation+0.25_turn).toDirection()*xOffset;
	bullet.update(deltaTimeFix, space);
	space->applyPhysics(&bullet, deltaTimeFix);
	space->spawnPlayerBullet(bullet);
}


void Ship::updateWeapon(float t, PlaySpace* space)
{
	if(IsShooting)
	{
		PrimaryWeapon.ShotTimer -= t;
		while(PrimaryWeapon.ShotTimer <= 0.f)
		{
			if(PrimaryWeapon.Bullets <= 1)
				shootBullet(space, PrimaryWeapon.BulletPrototype, -PrimaryWeapon.ShotTimer, 0, 0_turn);
			else if(PrimaryWeapon.Bullets == 2)
			{
				shootBullet(space, PrimaryWeapon.BulletPrototype, -PrimaryWeapon.ShotTimer, +10, 0_turn);
				shootBullet(space, PrimaryWeapon.BulletPrototype, -PrimaryWeapon.ShotTimer, -10, 0_turn);
			}
			else if(Modulo(PrimaryWeapon.Bullets, 2) == 1)
			{
				for(int i = -PrimaryWeapon.Bullets/2; i <= PrimaryWeapon.Bullets/2; ++i)
					shootBullet(space, PrimaryWeapon.BulletPrototype, -PrimaryWeapon.ShotTimer, i, PrimaryWeapon.Spread / PrimaryWeapon.Bullets * i);
			}
			else
				for(int i = -PrimaryWeapon.Bullets/2; i < PrimaryWeapon.Bullets; ++i)
					shootBullet(space, PrimaryWeapon.BulletPrototype, -PrimaryWeapon.ShotTimer, i*(3 + 14 / PrimaryWeapon.Bullets), 0_turn);
			PrimaryWeapon.ShotTimer += PrimaryWeapon.ShotDelay;
		}
	}
}

void Ship::onHit(Bullet* bullet, PlaySpace* space)
{
	if(ShieldEnergy <= 0)
		return;
	Particle p(ShieldParticle);
	reinterpret_cast<PhysicsObject&>(p) = reinterpret_cast<PhysicsObject&>(*this);
	p.Rotation = Angle(bullet->Position - Position);
	p.RotationSpeed = 0;
	p.Drag = 0;
	p.Flow = 0;
	p.Stabilizer = 0;
	p.Animation.Color.insert(0.05f, ShieldColors[Min(ShieldEnergy, 1.f) / MaxShield]);
	space->spawnParticle(p);
}

void Ship::draw(RenderContext r)
{
	if(Status == Destroyed)
		r.setColor(Colors::Grey);
	r.Offset = Position;
	r.Rotation = Rotation;
	Sprite.draw(r);
	r.setColor(FactionColor, 1.f);
	FactionColorSprite.draw(r);
}

void Ship::updateBounds()
{
	Bounds.Size = Sprite.getSize();
	PhysicsObject::updateBounds();
}

void Ship::doDamage(float damage)
{
	if(ShieldEnergy >= 0)
		ShieldEnergy -= damage;
	else
		Status = Destroyed;
}

Ship::~Ship()
{
	//delete AI;
}
