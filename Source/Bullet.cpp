//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Bullet.h"

Bullet::Bullet()
{
	ColorAnimation[0.0].insert(Colors::White);
	ColorAnimation[0.5].insert(Colors::Saturated::Orange);
	ColorAnimation[1.0].insert(Colors::Saturated::Red);
	
	AlphaAnimation[0.0].insert(1.f);
	AlphaAnimation[0.8].insert(1.f);
	AlphaAnimation[1.0].insert(0.f);
	
	ScaleAnimation[0.0].insert(1.f);
	ScaleAnimation[0.4].insert(1.f);
	ScaleAnimation[0.8].insert(Vec2F(1.f, 2.f));
	ScaleAnimation[1.0].insert(Vec2F(0.5f, 4.f));
}

void Bullet::update(float dt, PlaySpace* space)
{
	TimeSinceSpawn += dt;
}

void Bullet::draw(RenderContext r)
{
	r.Offset = Position;
	r.Rotation = Speed.getAngle();
	
	float age = normalizedAge();
	
	r.Scale = ScaleAnimation[age];
	r.setColor(ColorAnimation[age], AlphaAnimation[age]);
	Sprite.draw(r);
	r.setColor(Colors::White);
}
