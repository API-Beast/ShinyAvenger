//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "PhysicsObject.h"
#include <Springbok/Graphics/Image.h>
#include <Springbok/Animation/KeyframeList.h>

class Ship;

struct Bullet : public PhysicsObject
{
	int Faction = -1;
	float TimeSinceSpawn = 0.f;
	bool HitObject = false;
	
	struct _Definition
	{
		Image Sprite;
		Image Glow;
		
		float Lifetime = 2.f;
		float Power = 2.f;
		
		PhysicsObject PhysicsProperties;
		
		KeyframeList<float> AlphaAnimation = 1.0f;
		KeyframeList<Color> ColorAnimation = Colors::White;
		KeyframeList<Vec2F> ScaleAnimation = Vec2F(1.0f);
		
		KeyframeList<Color> GlowColorAnimation = Colors::White;
		KeyframeList<Vec2F> GlowScaleAnimation = Vec2F(1.f);
	} *Definition = nullptr;
	
	Bullet() = default;
	Bullet(Bullet::_Definition& def);
	
	virtual void update(float dt, PlaySpace* space);
	virtual void draw(RenderContext r);
	
	bool canBeDespawned(){ return TimeSinceSpawn > Definition->Lifetime || HitObject; };
	float normalizedAge(){ return TimeSinceSpawn / Definition->Lifetime; };
	void onHit(Ship* which, PlaySpace* space);
};

