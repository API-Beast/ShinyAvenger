//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <Springbok/Geometry/Angle.h>
#include <Springbok/Graphics/Image.h>
#include <Springbok/Procedural/RandomNumberGenerator.h>

#include "PhysicsObject.h"
#include "Bullet.h"
#include "Particle.h"

class PlaySpace;
class Behavior;

class Ship : public PhysicsObject
{
public:
	// General
	Image Sprite;
	float EngineAccleration = 400;
	
	// Particle Effects
	RandomNumberGenerator RNG;
	Particle ImpulseParticle;
	Particle SparkParticle;
	float ImpulseFXTimer = 0.f;
	
	// Diplomacy
	int Fraction = -1;
	ColorRGB FractionColor = Colors::White;
	
	// Controls
	float Steering = 0.0f;
	bool IsBraking = false;
	bool IsShooting = false;
	bool IsStabilizing = false;
	
	enum WeaponType
	{
		DoubleShot
	};
	// Weapon
	struct _Weapon
	{
		float ShotDelay = 0.15f;
		float ShotTimer = 0.0f;
		WeaponType Type = Ship::DoubleShot;
		Bullet BulletPrototype;
	} Weapon;
	
	// AI
	Behavior* AI = nullptr;
public:
	Ship();
	virtual ~Ship();
	virtual void update(float t, PlaySpace* space);	
	virtual void draw(RenderContext r);
	
	void updateControls(float t, PlaySpace* space);
	void updateWeapon(float t, PlaySpace* space);
	void updateFX(float t, PlaySpace* space);
};