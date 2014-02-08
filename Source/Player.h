//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once
#include <Springbok/Geometry/Angle.h>
#include <Springbok/Graphics/Image.h>
#include "PhysicsObject.h"

class Player : public PhysicsObject
{
public:
	Angle MovementDirection = 0;
	Angle TargetDirection = 0;
	float AcclerateFactor = 0.0f;
	Image Sprite;
public:
	Player() : Sprite("Player/Sprite.png"){};
	virtual void update(float t);
	virtual void draw(RenderContext r);
};