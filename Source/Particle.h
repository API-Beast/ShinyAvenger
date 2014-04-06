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
#include <Springbok/Graphics/Core/VertexStruct.h>

struct Particle : public PhysicsObject
{
	float TimeSinceSpawn = 0.f;
	Color Colorization = Colors::White;
	float Size = 1;
	float Alpha = 1;
	float LifeTimeMult = 1.0f;
	
	struct _Definition
	{
		Image Sprite;
		float Lifetime = 2.f;
		Blending DrawMode = Blending::Additive;
		PhysicsObject PhysicsProperties;
		struct
		{
			KeyframeList<float> Alpha = 1.f;
			KeyframeList<Color> Col = Colors::White;
			KeyframeList<Vec2F> Scale = Vec2F(1.f);
			KeyframeList<Angle> Rotation = 0.0_turn;
		} Animation;
	} *Definition = nullptr;
	
	Particle() = default;
	Particle(Particle::_Definition& def);
	
	virtual void update(float dt, PlaySpace* space);
	virtual void draw(BatchRenderer2D& r);
	
	bool canBeDespawned(){ return TimeSinceSpawn > (Definition->Lifetime * LifeTimeMult); };
	float normalizedAge(){ return TimeSinceSpawn / (Definition->Lifetime * LifeTimeMult); };
};