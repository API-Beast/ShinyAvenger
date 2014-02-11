//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "PhysicsObject.h"
#include <Springbok/Geometry/Angle.h>
#include <Springbok/Graphics/Image.h>
#include "PhysicsObject.h"
#include "Bullet.h"
#include "Particle.h"
#include <Springbok/Procedural/RandomNumberGenerator.h>

class Weapon;

class Ship : public PhysicsObject
{
public:
	Weapon TheWeapon;
	
	Image Sprite;
	
	float ImpulseTimer = 0.f;
	
	Particle ImpulseParticle;
	Particle SparkParticle;

	Movement *TheMovement;
	
	Behavior *Behavior;
	
	ColorRGB FractionColor;
	
public:
	Ship(Movement *TheMovement, ColorRGB FractionColor, Behavior *TheBehavior = NULL) : FractionColor(FractionColor), Behavior(TheBehavior) { }
	
	virtual void update(float t, PlaySpace* space);	
	virtual void draw(RenderContext r);
};

class Movement
{
	virtual void update(float t, Ship*, PlaySpace*);
};

class ControlledMovement : public Movement
{
public:
	float Steering = 0.0f;
	bool isBraking = false;
	
public:
	void update(float Time, Ship *TheShip, PlaySpace *Space)
	{
		if(isBraking)
			TheShip->Acceleration = 0;
		else
		{
			TheShip->Acceleration = TheShip->Rotation.toDirection() * 400;
			TheShip->ImpulseTimer -= Time;
			if(TheShip->ImpulseTimer < 0)
			{
				TheShip->ImpulseTimer = 0.05f;
				Particle glow(TheShip->ImpulseParticle);
				glow.Rotation = TheShip->Rotation;
				glow.Position = TheShip->Position + (TheShip->Rotation+0.5f).toDirection()*24;
				glow.Speed = -TheShip->Acceleration + TheShip->Speed;
				
				Space->spawnParticle(glow);
				
				for (int i = 0; i < 10; ++i)
				{
					Particle spark(TheShip->SparkParticle);
					spark.Rotation = TheShip->Rotation;
					spark.Position =TheShip-> Position + (TheShip->Rotation+0.5f).toDirection()*(14+RNG.generate()*10) + (TheShip->Rotation+0.25f).toDirection() * RNG.generate(-4.f, 4.f);
					spark.Speed = -TheShip->Acceleration + TheShip->Speed + (TheShip->Rotation+0.25f).toDirection() * RNG.generate(-60.f, 60.f);
					Space->spawnParticle(spark);
				}
			}
		}
		
		TheShip->RotationSpeed += Steering * t;
	}
};

class DestinationMovement : public Movement
{
	void update(float Time, Ship *TheShip, PlaySpace *Space)
	{
		
	}
}