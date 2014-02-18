//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Particle.h"

Particle::Particle(Particle::_Definition& def)
{
	PhysicsObject::operator=(def.PhysicsProperties);
	Definition = &def;
}

void Particle::draw(RenderContext r)
{
	float age = normalizedAge();
	r.Offset = Position;
	r.Rotation = Rotation + Definition->Animation.Rotation[age];
	r.Scale = Definition->Animation.Scale[age];
	r.setColor(Definition->Animation.Color[age], Definition->Animation.Alpha[age]);
	r.setBlendingMode(Definition->DrawMode);
	Definition->Sprite.draw(r);
}

void Particle::update(float dt, PlaySpace* space)
{
	TimeSinceSpawn += dt;
}
