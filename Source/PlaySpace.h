//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Space.h"
#include "Sector.h"
#include "PhysicsObject.h"
#include "GravitySource.h"
#include "Bullet.h"
#include "UI.h"
#include "Particle.h"
#include "Ship.h"

#include <Springbok/Geometry/Vec2.h>
#include <Springbok/Containers/List.h>
#include <Springbok/Platform/GameSurface.h>
#include <Springbok/Graphics/BitmapFont.h>

class Ship;

class PlaySpace : public Space
{
public:
	PlaySpace(GameSurface* surface);
	virtual ~PlaySpace();
	virtual void update(float time);
	virtual void draw();
	void applyPhysics(PhysicsObject* obj, float dt);
	void onMovementInput(bool up, bool down, bool right, bool left, float time);
	void onMouseHoldInput(Vec2F mousePos);
	void spawnPlayerBullet(Bullet bullet);
	void spawnParticle(Particle particle);
	void onActionInput(bool actionA, bool actionB, bool actionC);
	bool isHostile(Ship *, Ship*);
	bool isHostile(int, int);
	Sector* generateSector(Vec2F position);
	Color getFactionColor(int);
	
	// TODO For now just return all Ships, this is just so I can swap it out later without redoing all the code
	List<Ship*>& findShips(Vec2F topLeft, Vec2F bottomRight){ return Ships; };
public:
	Ship* Player;
	
	Sector* HomeSector;
	
	Image BackgroundStars;
	Image BackgroundFog;
	Image BackgroundFogB;
	Image ForegroundFog;
	
	RandomNumberGenerator WorldRNG;
	
	BitmapFont BigFont;
	BitmapFont SmallFont;
	
	List<PhysicsObject*> Objects;
	
	List<Bullet> Bullets = List<Bullet>(512);
	List<GravitySource> GravitySources;
	List<Particle> Particles = List<Particle>(4096);
	int SoftMaxParticleCount = 1024;

	List<Ship*> Ships;
	List<Sector*> Sectors;

	KeyframeList<Color> BackgroundGradient;
	UIContainer GUIContainer;
	
	Vec2F CameraPos;
	Vec2F ScreenSize;
	
	Vec2I Size;

	float AirDrag = 0.001f;
	float RotationAirDrag = 0.01f;
	float GameTime = 0.0f;
	float LastDeltaTime = 0.0f;
};