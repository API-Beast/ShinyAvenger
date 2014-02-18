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
	PhysicsObject* CurrentTarget = nullptr; // Not used by the standard definition
	
	struct _Definition
	{
		Image Sprite;
		Image Glow;
		
		float Lifetime = 2.f;
		float Power = 2.f;
		
		float Speed = 600.f;
		
		bool Explodes = false;
		
		PhysicsObject PhysicsProperties;
		
		KeyframeList<float> AlphaAnimation = 1.0f;
		KeyframeList<Color> ColorAnimation = Colors::White;
		KeyframeList<Vec2F> ScaleAnimation = Vec2F(1.0f);
		
		KeyframeList<Color> GlowColorAnimation = Colors::White;
		KeyframeList<Vec2F> GlowScaleAnimation = Vec2F(1.f);
		
		virtual void update(Bullet& bullet, float dt, PlaySpace* space){};
		virtual void onHit(Bullet& bullet, Ship* which, PlaySpace* space){};
		
		_Definition& operator=(const _Definition&) = default;
	} *Definition = nullptr;
	
	Bullet() = default;
	Bullet(Bullet::_Definition& def);
	
	virtual void update(float dt, PlaySpace* space);
	virtual void draw(RenderContext r);
	
	bool canBeDespawned(){ return TimeSinceSpawn > Definition->Lifetime || HitObject; };
	float normalizedAge(){ return TimeSinceSpawn / Definition->Lifetime; };
	void onHit(Ship* which, PlaySpace* space);
};

struct MissileDefinition : public Bullet::_Definition
{
	float RotationRate = 2;
	float Acceleration = 1000;
	
	float TargetingRange = 1000;
	Angle TargetingCone = 0.3_turn;
	
	virtual void update(Bullet& b, float dt, PlaySpace* space);
};