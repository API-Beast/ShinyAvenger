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
#include "Leader.h"

class PlaySpace;
class Behavior;

class Ship : public PhysicsObject, public Leader
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
	Image Pictogram;
	Image PictogramOverlay;
	
	float EngineAccleration = 400;
	float  BoostAccleration = 800;
	
	float ShieldEnergy = 100;
	float MaxShield = 100;
	float ShieldRegeneration = 10;
	ShipState Status = Ship::Okay;
	float Age = 0;
	float BoostFuel = 1.f;
	float BoostMaxFuel = 1.f;
	float BoostRecharge = 0.1f;
	
	// Particle Effects
	float ImpulseTimer = 0.f;
	float ImpulseSparkTimer = 0.f;
	KeyframeList<Color> ShieldColors = {{1.0f, Colors::White}, {0.5f, Colors::Saturated::Cyan}, {0.0f, Colors::Saturated::Red}};
	Color ImpulseColor = Palette::Orange;
	
	// Diplomacy
	int Faction = -1;
	Color FactionColor = Colors::White;
	Image Overlay;
	
	// Controls
	float Steering = 0.0f;
	bool IsBraking = false;
	bool IsShooting = false;
	bool IsShootingSecondary = false;
	bool IsStabilizing = false;
	bool IsBoosting = false;
	
	// Sounds
	bool boostFlag = false;
	
	enum WeaponType
	{
		MultiShot
	};
	// Weapon
	struct _Weapon
	{
		float ShotDelay = 0.15f;
		float ShotTimer = 0.0f;
		
		float ReloadDelay = 0.10f;
		float ReloadTimer = 0.0f;
		int MaxReloadStack = 2;
		int CurReloadStack = 2;
		
		WeaponType Type = Ship::MultiShot;
		Bullet BulletPrototype;
		int Bullets = 2;
		Angle Spread = 0_turn;
		float MuzzleFlashSize = 0;
		Color MuzzleFlashColor = Colors::White;
	} PrimaryWeapon;
	
	_Weapon SecondaryWeapon;
	
	// AI
	Behavior* AI = nullptr;
public:
	virtual ~Ship();
	virtual void update(float t, PlaySpace* space);	
	virtual void draw(SpriteRenderer& r);
	virtual void drawPictogram(SpriteRenderer& r, float sizeMult);
	virtual void updateBounds();
	bool canBeDespawned(){ return Status == Destroyed; };
	virtual Vec2F getPosition() const;
	void doDamage(float damage, PlaySpace* space);
	void updateControls(float t, PlaySpace* space);
	void updateWeapon(Ship::_Weapon& weapon, float t, PlaySpace* space, bool shooting);
	void shootBullet(PlaySpace* space, Bullet prototype, float deltaTimeFix, int xOffset, Angle rotation);
	void updateFX(float t, PlaySpace* space);
	void onHit(Bullet* bullet, PlaySpace* space);
	bool hasMoved() const { return Speed.length() > 0; }
	bool isDestroyed() const { return Status == Destroyed; }
	bool isLeader() const { return Group.UsedLength > 0; }
};