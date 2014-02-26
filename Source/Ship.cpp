//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Ship.h"
#include "Behavior.h"
#include "PlaySpace.h"
#include "AssetDefinition.h"

Ship::Ship(const Image& img) : Sprite(img)
{	
	FactionColorSprite = Image("Player/FactionColor.png");
	
	ShieldParticleDef = gAssets.EnergyShield;
	ShieldColors.insert(1.0f, Colors::White);
	ShieldColors.insert(0.5f, Colors::Saturated::Cyan);
	ShieldColors.insert(0.0f, Colors::Saturated::Red);
}

void Ship::update(float t, PlaySpace* space)
{	
	Age += t;
	if(AI)
		AI->update(t, this, space);
	
	if(Status != Destroyed)
	{
		if(ShieldEnergy < MaxShield)
			ShieldEnergy = Approach(ShieldEnergy, MaxShield, t * ShieldRegeneration);
		
		updateControls(t, space);
		updateFX      (t, space);
		
		if(IsShooting)
			updateWeapon(PrimaryWeapon, t, space);
		if(IsShootingSecondary)
			updateWeapon(SecondaryWeapon, t, space);
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
		
		Particle glow(gAssets.PulseEngineGlow);
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
		
		Particle spark(gAssets.PulseEngineSpark);
		spark.Rotation = Rotation;
		spark.Position = Position - Rotation.toDirection()*(16+RNG.generate()*8) + (Rotation+0.25_turn).toDirection() * RNG.generate(-4.f, 4.f);
		spark.Speed = -Acceleration + Speed + (Rotation+0.25_turn).toDirection() * RNG.generate(-60.f, 60.f);
		
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
	bullet.Speed = (Rotation + rotation ).toDirection() * bullet.Definition->Speed + Speed *0.8;
	bullet.Position = Position + (Rotation+0.25_turn).toDirection()*xOffset;
	bullet.update(deltaTimeFix, space);
	space->applyPhysics(&bullet, deltaTimeFix);
	space->spawnPlayerBullet(bullet);
}


void Ship::updateWeapon(Ship::_Weapon& weapon, float t, PlaySpace* space)
{
	weapon.ShotTimer -= t;
	while(weapon.ShotTimer <= 0.f)
	{
		if(weapon.MuzzleFlashSize)
		{
			Particle muzzleFlash(gAssets.GlowParticle);
			muzzleFlash.Position = this->Position;
			muzzleFlash.Size = weapon.MuzzleFlashSize;
			muzzleFlash.Colorization = weapon.MuzzleFlashColor;
			muzzleFlash.Alpha = 0.1f;
			space->spawnParticle(muzzleFlash);
		}
		
		if(weapon.Bullets <= 1)
			shootBullet(space, weapon.BulletPrototype, -weapon.ShotTimer, 0, RNG.generate(-1.0f, +1.0f) * weapon.Spread);
		else if(weapon.Bullets == 2)
		{
			shootBullet(space, weapon.BulletPrototype, -weapon.ShotTimer, +10, +weapon.Spread / 2);
			shootBullet(space, weapon.BulletPrototype, -weapon.ShotTimer, -10, -weapon.Spread / 2);
		}
		else if(Modulo(weapon.Bullets, 2) == 1)
		{
			for(int i = -weapon.Bullets/2; i <= weapon.Bullets/2; ++i)
				shootBullet(space, weapon.BulletPrototype, -weapon.ShotTimer, i*(3 + 14 / weapon.Bullets), weapon.Spread / weapon.Bullets * i);
		}
		else
			for(int i = -weapon.Bullets/2; i < weapon.Bullets/2; ++i)
				shootBullet(space, weapon.BulletPrototype, -weapon.ShotTimer, i*(3 + 14 / weapon.Bullets), weapon.Spread / weapon.Bullets * i);
		
		weapon.ShotTimer += weapon.ShotDelay;
	}
}

void Ship::draw(RenderContext r)
{
	if(Status == Destroyed)
		r.setColor(Palette::Grey[1]);
	
	r.Offset = Position;
	r.Rotation = Rotation;
	Sprite.draw(r);
	r.setColor(FactionColor, 1.f);
	FactionColorSprite.draw(r);
	
	if(ShieldEnergy < (MaxShield * 0.8f) && ShieldEnergy > 0.f)
	{
		float percentShield = (Max(ShieldEnergy, 1.f) / (MaxShield * 0.8f));
		r.setBlendingMode(RenderContext::Additive);
		r.setColor(ShieldColors[percentShield], 1.0f * (1 - percentShield));
		r.Rotation = 0_turn;
		gAssets.ShieldStaticSprite.draw(r);
		
		r.setColor(ShieldColors[percentShield+0.2f], Max(Angle::FromTurn(Age).cos(), 0.) * (1 - percentShield));
		r.Rotation = Angle::FromDegree(int(Age)*60);
		gAssets.ShieldRechargeSprite.draw(r);
	}
}

void Ship::updateBounds()
{
	Bounds.Size = Sprite.getSize();
	PhysicsObject::updateBounds();
}

void Ship::onHit(Bullet* bullet, PlaySpace* space)
{
	bool shield = false;
	if(ShieldEnergy > 0)
	{
		shield = true;
		
		Particle p(ShieldParticleDef);
		reinterpret_cast<PhysicsObject&>(p) = reinterpret_cast<PhysicsObject&>(*this);
		p.Rotation = Angle(bullet->Position - Position);
		p.RotationSpeed = 0;
		p.Drag = 0;
		p.Flow = 0;
		p.Stabilizer = 0;
		space->spawnParticle(p);
	}
	
	bullet->onHit(this, space);
	if(shield && ShieldEnergy < 0)
	{
		Particle p(gAssets.EnergyShieldDestruction);
		reinterpret_cast<PhysicsObject&>(p) = reinterpret_cast<PhysicsObject&>(*this);
		p.Rotation = Angle(bullet->Position - Position);
		p.RotationSpeed = 0;
		p.Drag = 0;
		p.Flow = 0;
		p.Stabilizer = 0;
		space->spawnParticle(p);
	}
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
	if (AI != nullptr)	
		delete AI;
}


Vec2F Ship::getPosition() const
{
	return Position;
}
