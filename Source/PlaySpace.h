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
#include "Particle.h"
#include "Ship.h"

#include <Springbok/Geometry/Vec2.h>
#include <Springbok/Geometry/GeometryView.h>
#include <Springbok/Containers/List.h>
#include <Springbok/Platform/GameSurface.h>
#include <Springbok/Audio/SoundInstance.h>
#include <Springbok/Graphics/SpriteRenderer.h>
#include <Springbok/Graphics/Framebuffer.h>

class Ship;

struct Sector
{
	Vec2I Position;
	bool Generated = false;
};

class PlaySpace : public Space
{
public:
	PlaySpace() : Renderer(nullptr) {};
	PlaySpace(GameSurface* surface, const List<std::string>& arguments);
	virtual ~PlaySpace();
	virtual void update(float time);
	virtual void draw();
	void drawHUD();
	virtual bool isFinished(){ return TimeSincePlayerDestruction > 2; };
	void applyPhysics(PhysicsObject* obj, float dt);
	void onMovementInput(bool up, bool down, bool right, bool left, float time);
	void onMouseHoldInput(Vec2F mousePos);
	void onScrollInput(Vec2F scroll);
	
	void spawnPlayerBullet(Bullet bullet);
	void spawnParticle(Particle particle, bool important = false);
	
	void spawnParticle(Particle def, Vec2F position, float scale, Angle rotation, Color col, float lifetime = 1.f, bool important = false);
	void castParticles(const Particle& def, Vec2F position, int amount, RangeF power, Color col = Colors::White, RangeF scale = 1.f, RangeF lifetime = 1.f, float turbulenceFactor = 1.0f);
	
	void spawnExplosion(Vec2F position, float size, float force, Color sparkColor);
	void onActionInput(bool actionA, bool actionB, bool actionC, bool boost);
	bool isHostile(Ship *, Ship*);
	bool isHostile(int, int);
	void checkSectorGeneration(Vec2F position);
	SolarSystem* generateSystem(Vec2F position, int faction);
	Color getFactionColor(int);
	List< Ship* > findShips(Vec2F topLeft, Vec2F bottomRight);
public:
	ObjectPointer<Ship> Player;
	
	SolarSystem* HomeSystem;
	
	Image BackgroundStars;
	Image BackgroundFog;
	Image BackgroundFogB;
	Image ForegroundFog;
	
	RandomNumberGenerator WorldRNG;
	
	List<PhysicsObject*> Objects;
	
	List<Bullet> Bullets = List<Bullet>(512);
	List<GravitySource> GravitySources;
	List<Particle> Particles = List<Particle>(4096);
	List<Ship*> Ships;
	List<SolarSystem*> Systems;
	
	int SoftMaxParticleCount = 1024;
	
	Map<Sector, Vec2I, &Sector::Position> Sectors;
	SpriteRenderer Renderer;
	Framebuffer* HDRTarget;
	
	Vec2F CameraPos;
	
	Vec2I Size;

	float AirDrag = 0.001f;
	float RotationAirDrag = 0.02f;
	
	Vec2F SectorSize = 17500;
	float SectorLookAhead = 20000;
	
	int NumberGeneratedSectors = 0;
	
	// Sounds
	SoundInstance* Music;
	
	int GameFrame = 0;
	float GameTime = 0.0f;
	float TimeSincePlayerDestruction = 0.f;
	float LastDeltaTime = 0.0f;
	
	int ParticleBudget = 0;
	
	bool IsStressTesting = false;
};