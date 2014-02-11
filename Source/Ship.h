//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "PhysicsObject.h"

class Weapon;

class Ship : public PhysicsObject
{
public:
	Weapon TheWeapon;
	Image Sprite;
	Particle ImpulseParticle;
	
public:
	Ship();
	
	virtual void update(float t, PlaySpace* space);	
	virtual void draw(RenderContext r);
};