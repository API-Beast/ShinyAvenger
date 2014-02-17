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
	float TimeSinceSpawn = 0.f;
	
	struct _Definition
	{
		Image Sprite;
		float Lifetime = 2.f;
		RenderContext::BlendingMode DrawMode = RenderContext::Additive;
		PhysicsObject PhysicsProperties;
		struct
		{
			KeyframeList<float> Alpha = 1.f;
			KeyframeList<ColorRGB> Color = Colors::White;
			KeyframeList<Vec2F> Scale = Vec2F(1.f);
			KeyframeList<Angle> Rotation = 0.0_turn;
		} Animation;
	} *Definition = nullptr;
	
	Particle() = default;
	Particle(Particle::_Definition& def);
	virtual void update(float dt, PlaySpace* space);
	virtual void draw(RenderContext r);
	bool canBeDespawned(){ return TimeSinceSpawn > Definition->Lifetime; };
	float normalizedAge(){ return TimeSinceSpawn / Definition->Lifetime; };
};