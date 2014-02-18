//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Bullet.h"
#include "Ship.h"
#include "PlaySpace.h"

void Bullet::update(float dt, PlaySpace* space)
{
	TimeSinceSpawn += dt;
	Definition->update(*this, dt, space);
}

void Bullet::draw(RenderContext r)
{
	r.Offset = Position;
	r.Rotation = Rotation;
	
	float age = normalizedAge();

	r.setColor(Definition->ColorAnimation[age], Definition->AlphaAnimation[age]);
	r.Scale = Definition->ScaleAnimation[age];
	Definition->Sprite.draw(r);
	
	r.setColor(Definition->GlowColorAnimation[age], Definition->AlphaAnimation[age]/20);
	r.Scale = Definition->GlowScaleAnimation[age];
	r.setBlendingMode(RenderContext::Additive);
	Definition->Glow.draw(r);
}

Bullet::Bullet(Bullet::_Definition& def)
{
	PhysicsObject::operator=(def.PhysicsProperties);
	Definition = &def;
}

void Bullet::onHit(Ship* which, PlaySpace* space)
{
	HitObject = true;
	which->doDamage(Definition->Power);
}

void MissileDefinition::update(Bullet& b, float dt, PlaySpace* space)
{	
	// Enemy out of range?
	if(b.CurrentTarget)
		if((b.CurrentTarget->Position - b.Position).getLength() > TargetingRange * 1.5f)
			b.CurrentTarget = nullptr;
	
	// Find a new target
	if(!b.CurrentTarget)
	{
		int minRange = TargetingRange;
		for(Ship* ship : space->findShips(b.Position - 250, b.Position + 250))
		{
			float distance = (ship->Position - b.Position).getLength();
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
		b.Acceleration = b.Rotation.toDirection() * Acceleration;
		Angle targetAngle = Angle(b.CurrentTarget->Position - b.Position);
		b.Rotation = Approach(b.Rotation, targetAngle, Angle(RotationRate*dt));
	}
}
