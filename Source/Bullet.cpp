//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Bullet.h"
#include "Ship.h"
#include "PlaySpace.h"
#include "AssetDefinition.h"
#include <Springbok/Graphics/SpriteRenderer.h>

void Bullet::update(float dt, PlaySpace* space)
{
	TimeSinceSpawn += dt;
	if(TimeSinceSpawn > Definition->Lifetime)
		impact(space);
	
	Definition->update(*this, dt, space);
}

void Bullet::impact(PlaySpace* space)
{
	if(Definition->Explodes && !Exploded)
	{
		space->spawnExplosion(Position, Definition->Power*4, Definition->Power*10, Colors::Oxygen::Orange);
		Exploded = true;
	}
}

void Bullet::draw(SpriteRenderer& r)
{
	float age = normalizedAge();
	Vec4F col = Vec4F(Definition->ColorAnimation[age], Definition->AlphaAnimation[age]);
	
	Transform2D t = Position2D(Position);
	t += Rotate2D(Rotation);
	t += Scale2D(Definition->ScaleAnimation[age]);
	
	r.draw(Definition->Sprite, t, col, Definition->BlendMode);
}

void Bullet::drawTop(SpriteRenderer& r)
{
	if(Definition->Glow)
	{
		float age   = normalizedAge();
		Vec3F col   = Definition->ColorAnimation[age];
		float alpha = Definition->AlphaAnimation[age];
		
		Transform2D t = Position2D(Position);
		
		r.draw(gAssets.GlowSprite, Rotate2D(Rotation) + t + Scale2D((gAssets.GlowSprite.size() / Definition->Sprite.size()) * 0.60f * Definition->ScaleAnimation[age]), {col, alpha * 0.10f}, Blending::Additive);
		r.draw(gAssets.GlowSprite, Rotate2D(Rotation) + t + Scale2D((gAssets.GlowSprite.size() / Definition->Sprite.size()) * 0.10f * Definition->ScaleAnimation[age]), {col, alpha * 0.40f}, Blending::Additive);
	}
}


Bullet::Bullet(Bullet::_Definition& def)
{
	PhysicsObject::operator=(def.PhysicsProperties);
	Definition = &def;
}

void Bullet::onHit(Ship* which, PlaySpace* space)
{
	HitObject = true;
	which->doDamage(Definition->Power, space);
	if(Definition->Explodes)
	{
		impact(space);
	}
	else
	{
		Angle ang = Angle((this->Position - this->Speed * 0.1f) - which->Position);
		int particles = gRNG.getNumber(1, 2);
		float age = normalizedAge();
		for(int i = 0; i < particles; ++i)
		{
			Angle angle = ang + 0.4_turn * gRNG.getNumber(-1.f, +1.f);
			Particle spark(gAssets.SparkParticle);
			spark.Position = this->Position;
			spark.Speed = angle.toDirection() * 200;
			spark.Rotation = angle;
			spark.Colorization = Definition->ColorAnimation[age];
			space->spawnParticle(spark);
		}
		Particle glow(gAssets.GlowParticle);
		glow.Position = this->Position;
		glow.Size = 4.f;
		glow.Colorization = Definition->ColorAnimation[age];
		space->spawnParticle(glow);
	}
}

void MissileDefinition::update(Bullet& b, float dt, PlaySpace* space)
{	
	// Enemy out of range?
	if(b.CurrentTarget)
		if(!InRange(b.CurrentTarget->Position, b.Position, TargetingRange * 1.5f))
			b.CurrentTarget = nullptr;
	
	// Find a new target
	if(!b.CurrentTarget)
	{
		int minRange = TargetingRange;
		for(Ship* ship : space->findShips(b.Position - TargetingRange, b.Position + TargetingRange))
		{
			float distance = Distance(ship->Position, b.Position);
			if(distance < minRange
			 && space->isHostile(b.Faction, ship->Faction)
			 && ship->Status != Ship::Destroyed)
			{
				minRange = distance;
				b.CurrentTarget = ship;
			}
		}
	}
	
	// Found one? Align.
	if(b.CurrentTarget)
	{
		Angle targetAngle = Angle(b.CurrentTarget->Position - b.Position);
		b.Rotation = Approach(b.Rotation, targetAngle, Angle(RotationRate*dt));
	}
	b.Acceleration = b.Rotation.toDirection() * Acceleration;
}
