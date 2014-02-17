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
	enum ShipState
	{
		Okay,        // All systems intact
		Compromised, // Systems broken, but may malfunction
		Destroyed    // Lifeless
	};
	
	// General
	Image Sprite;
	float EngineAccleration = 400;
	
	float ShieldEnergy = 100;
	float MaxShield = 100;
	float ShieldRegeneration = 0;
	ShipState Status = Ship::Okay;
	
	// Particle Effects
	RandomNumberGenerator RNG;
	Particle ImpulseParticle;
	Particle SparkParticle;
	float ImpulseTimer = 0.f;
	float ImpulseSparkTimer = 0.f;
	KeyframeList<Color> ShieldColors;
	
	// Diplomacy
	int Faction = -1;
	ColorRGB FactionColor = Colors::White;
	Image FactionColorSprite;
	
	// Controls
	float Steering = 0.0f;
	bool IsBraking = false;
	bool IsShooting = false;
	bool IsStabilizing = false;
	
	enum WeaponType
	{
		MultiShot
	};
	// Weapon
	struct _Weapon
	{
		float ShotDelay = 0.15f;
		float ShotTimer = 0.0f;
		WeaponType Type = Ship::MultiShot;
		Bullet BulletPrototype;
		int Bullets = 2;
		Angle Spread = 0_turn;
	} PrimaryWeapon;
	
	_Weapon SecondaryWeapon;
	
	// AI
	Behavior* AI = nullptr;
	Particle ShieldParticle;
public:
	Ship(const Image& img);
	virtual ~Ship();
	virtual void update(float t, PlaySpace* space);	
	virtual void draw(RenderContext r);
	virtual void updateBounds();
	bool canBeDespawned(){ return Status == Destroyed; };
	
	void doDamage(float damage);
	void updateControls(float t, PlaySpace* space);
	void updateWeapon(float t, PlaySpace* space);
	void shootBullet(PlaySpace* space, Bullet prototype, float deltaTimeFix, int xOffset, Angle rotation);
	void updateFX(float t, PlaySpace* space);
	void onHit(Bullet* bullet, PlaySpace* space);
};