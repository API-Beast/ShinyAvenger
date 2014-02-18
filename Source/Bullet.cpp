//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Bullet.h"
#include "Ship.h"

void Bullet::update(float dt, PlaySpace* space)
{
	TimeSinceSpawn += dt;
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