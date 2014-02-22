//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Space.h"
#include "SolarSystem.h"
#include "PhysicsObject.h"
#include "GravitySource.h"
#include "Bullet.h"
#include "UI.h"
#include "Particle.h"
#include "Ship.h"

#include <Springbok/Geometry/Vec2.h>
#include <Springbok/Containers/List.h>
#include <Springbok/Platform/GameSurface.h>

class Ship;

struct Sector
{
	Vec2I Position;
	bool Generated = false;
};

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
	void checkSectorGeneration(Vec2F position);
	SolarSystem* generateSystem(Vec2F position, int faction);
	Color getFactionColor(int);
	
	// TODO For now just return all Ships, this is just so I can swap it out later without redoing all the code
	List<Ship*>& findShips(Vec2F topLeft, Vec2F bottomRight){ return Ships; };
public:
	Ship* Player;
	
	SolarSystem* HomeSector;
	
	Image BackgroundStars;
	Image BackgroundFog;
	Image BackgroundFogB;
	Image ForegroundFog;
	
	RandomNumberGenerator WorldRNG;
	
	List<PhysicsObject*> Objects;
	
	List<Bullet> Bullets = List<Bullet>(512);
	List<GravitySource> GravitySources;
	List<Particle> Particles = List<Particle>(4096);
	int SoftMaxParticleCount = 1024;

	List<Ship*> Ships;
	List<SolarSystem*> Systems;
	
	Map<Sector, Vec2I, &Sector::Position> Sectors;

	KeyframeList<Color> BackgroundGradient;
	
	Vec2F CameraPos;
	Vec2F ScreenSize;
	
	Vec2I Size;

	float AirDrag = 0.001f;
	float RotationAirDrag = 0.01f;
	float GameTime = 0.0f;
	float LastDeltaTime = 0.0f;
	
	Vec2F SectorSize = 10000;
	float SectorLookAhead = 5000;
	
	// User Interface
	Arrows ShipArrows;
	Label FrameRate;
	ShieldBar Shield;
	UIContainer GUIContainer;
};