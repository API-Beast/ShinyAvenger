//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Particle.h"
#include <Springbok/Graphics/Core/Transform2D.h>
#include <Springbok/Graphics/Core/BatchRenderer.h>

Particle::Particle(Particle::_Definition& def)
{
	PhysicsObject::operator=(def.PhysicsProperties);
	Definition = &def;
}

void Particle::draw(BatchRenderer2D& r)
{
	float age = normalizedAge();
	
	Vec4F color;
	color.XYZ = Colorization * Definition->Animation.  Col[age];
	color.W   = Alpha        * Definition->Animation.Alpha[age];
	
	Transform2D t = Position2D(Position);
	t += Rotate2D(Rotation + Definition->Animation.Rotation[age]);
	t +=  Scale2D(Size     * Definition->Animation.   Scale[age]);
	
	r.draw(Definition->Sprite, t, color, Definition->DrawMode);
}

void Particle::update(float dt, PlaySpace* space)
{
	TimeSinceSpawn += dt;
}
