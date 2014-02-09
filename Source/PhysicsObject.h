//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <Springbok/Geometry/Vec2.h>
#include <Springbok/Graphics/RenderContext.h>

class PlaySpace;

struct PhysicsObject
{
	Vec2F Position = 0;
	Vec2F Speed = 0;
	Vec2F Acceleration = 0;
	Vec2F Drag = 1;
	Vec2F Mass = 1;
	
	Angle Rotation = 0;
	float RotationSpeed = 0.0f;
public:
	virtual void draw(RenderContext r){};
	virtual void update(float dt, PlaySpace* space){};
};