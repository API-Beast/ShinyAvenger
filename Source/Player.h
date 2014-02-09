//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once
#include <Springbok/Geometry/Angle.h>
#include <Springbok/Graphics/Image.h>
#include "PhysicsObject.h"
#include "Bullet.h"

class Player : public PhysicsObject
{
public:
	float Steering = 0.0f;
	float AcclerateFactor = 0.0f;
	
	bool IsShooting = false;
	float ShootTimer = 0.f;
	
	Image Sprite = Image("Player/Sprite.png");
	
	Bullet BulletPrototype;
public:
	Player();
	virtual void update(float t, PlaySpace* space);
	virtual void draw(RenderContext r);
};