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
	if (!this->IsBraking && boostFlag != !this->IsBraking)
	{
		// Boost sound
		gAssets.SoundBoost->MinRange = 50;
		gAssets.SoundBoost->Pitch = gRNG.getFloat() * 0.4f + 0.9f;
		gAssets.SoundBoost->Volume = gRNG.getFloat() * 0.2f + 0.4f;
		SoundInstance *sound = gAssets.SoundBoost->play(Position);			
	}
	
	Age += t;
	if(AI)
		AI->update(t, this, space);
	
	if(Status != Destroyed)
	{
		if(ShieldEnergy < MaxShield)
			ShieldEnergy = Approach(ShieldEnergy, MaxShield, t * ShieldRegeneration);
		
		updateControls(t, space);
		updateFX      (t, space);
		
		updateWeapon(PrimaryWeapon, t, space, IsShooting);
		updateWeapon(SecondaryWeapon, t, space, IsShootingSecondary);
	}
	else
		Acceleration = 0;
	
	boostFlag = !this->IsBraking;
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
		glow.Position = Position - Rotation.toDirection()*12;
		glow.Speed = -Acceleration + Speed;
		glow.Colorization = ImpulseColor;
		glow.update(supplementT, space);
		space->applyPhysics(&glow, supplementT);
		space->spawnParticle(glow);
		ImpulseTimer += 0.025f;
	}
	
	while(ImpulseSparkTimer < 0)
	{
		// Spawn sparks
		float supplementT = -ImpulseSparkTimer;
		if(supplementT < 0)
			supplementT = 0;
		
		Particle spark(gAssets.PulseEngineSpark);
		spark.Rotation = Rotation;
		spark.Position = Position - Rotation.toDirection()*12 + (Rotation+0.25_turn).toDirection() * gRNG.getNumber(-4.f, 4.f);
		spark.Speed = -Acceleration + Speed + (Rotation+0.25_turn).toDirection() * gRNG.getNumber(-60.f, 60.f);
		spark.Colorization = ImpulseColor;
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


void Ship::updateWeapon(Ship::_Weapon& weapon, float t, PlaySpace* space, bool shooting)
{
	
	weapon.ReloadTimer -= t;
	if(weapon.ReloadTimer <= 0.f)
	{
		weapon.ReloadTimer = weapon.ReloadDelay;
		weapon.CurReloadStack = Min(weapon.CurReloadStack+1, weapon.MaxReloadStack);
	}
	
	if(shooting && (weapon.CurReloadStack > 0))
	{
		weapon.ShotTimer -= t;
		while(weapon.ShotTimer <= 0.f)
		{				
			weapon.CurReloadStack -= 1;
			
			if(weapon.Bullets <= 1)
				shootBullet(space, weapon.BulletPrototype, -weapon.ShotTimer, 0, gRNG.getNumber(-1.0f, +1.0f) * weapon.Spread);
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
			
			if(weapon.MuzzleFlashSize)
			{
				Particle muzzleFlash(gAssets.GlowParticle);
				muzzleFlash.Position = this->Position;
				muzzleFlash.Size = weapon.MuzzleFlashSize;
				muzzleFlash.Colorization = weapon.MuzzleFlashColor;
				muzzleFlash.Alpha = 0.1f;
				space->spawnParticle(muzzleFlash);
			}
			
			// Sounds!
			gAssets.SoundHeavyShot->MinRange = 50;
			gAssets.SoundHeavyShot->Pitch = gRNG.getFloat() * 0.2f + 0.8f;
			gAssets.SoundHeavyShot->Volume = gRNG.getFloat() * 0.2f + 0.5f;
			gAssets.SoundHeavyShot->play(Position);	
			
			gAssets.SoundSimpleShot->MinRange = 50;
			gAssets.SoundSimpleShot->Pitch = gRNG.getFloat() * 0.15f + 0.99f;
			gAssets.SoundSimpleShot->Volume = gRNG.getFloat() * 0.1f + 0.1f;
			gAssets.SoundSimpleShot->play(Position);	
		}
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
		space->spawnParticle(p, true);
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
		space->spawnParticle(p, true);
		gAssets.SoundExplosion01->MinRange = 50;
		gAssets.SoundExplosion01->Pitch = gRNG.getFloat() * 0.2f + 0.7f;
		gAssets.SoundExplosion01->Volume = gRNG.getFloat() * 0.2f + 0.5f;
		gAssets.SoundExplosion01->play(Position);
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
