//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "PhysicsObject.h"
#include <Springbok/Graphics/Image.h>
#include <Springbok/Animation/KeyframeList.h>

struct Bullet : public PhysicsObject
{
	Image Sprite;
	Image Glow;
	
	float TimeSinceSpawn = 0.f;
	float Lifetime = 2.f;
	
	KeyframeList<float> AlphaAnimation;
	KeyframeList<Color> ColorAnimation;
	KeyframeList<Vec2F> ScaleAnimation;
	
	KeyframeList<Color> GlowColorAnimation;
	KeyframeList<Vec2F> GlowScaleAnimation;
	
	virtual void update(float dt, PlaySpace* space);
	virtual void draw(RenderContext r);
	bool canBeDespawned(){ return TimeSinceSpawn > Lifetime; };
	float normalizedAge(){ return TimeSinceSpawn / Lifetime; };
};

