//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <Springbok/Animation/KeyframeList.h>
#include <Springbok/Graphics/Color.h>
#include <Springbok/Graphics/Image.h>
#include "PhysicsObject.h"

struct Particle : public PhysicsObject
{
	Image Sprite;
	RenderContext::BlendingMode DrawMode = RenderContext::Additive;
	
	float TimeSinceSpawn = 0.f;
	float Lifetime = 2.f;
	
	struct
	{
		KeyframeList<float> Alpha;
		KeyframeList<ColorRGB> Color;
		KeyframeList<Vec2F> Scale;
		KeyframeList<Angle> Rotation;
	} Animation;
	
	virtual void update(float dt, PlaySpace* space);
	virtual void draw(RenderContext r);
	bool canBeDespawned(){ return TimeSinceSpawn > Lifetime; };
	float normalizedAge(){ return TimeSinceSpawn / Lifetime; };
};