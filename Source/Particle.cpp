//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Particle.h"

void Particle::draw(RenderContext r)
{
	float age = normalizedAge();
	r.Offset = Position;
	r.Rotation = Rotation + Animation.Rotation[age];
	r.Scale = Animation.Scale[age];
	r.setColor(Animation.Color[age], Animation.Alpha[age]);
	r.setBlendingMode(DrawMode);
	Sprite.draw(r);
}

void Particle::update(float dt, PlaySpace* space)
{
	TimeSinceSpawn += dt;
}
