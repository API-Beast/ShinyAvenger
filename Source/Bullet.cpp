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

	r.setColor(ColorAnimation[age], AlphaAnimation[age]);
	r.Scale = ScaleAnimation[age];
	Sprite.draw(r);
	
	r.setColor(GlowColorAnimation[age], AlphaAnimation[age]/20);
	r.Scale = GlowScaleAnimation[age];
	r.setBlendingMode(RenderContext::Additive);
	Glow.draw(r);
}

void Bullet::onHit(Ship* which, PlaySpace* space)
{
	HitObject = true;
}