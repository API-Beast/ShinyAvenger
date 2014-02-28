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
#include <Springbok/Geometry/GeometryView.h>
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
	PlaySpace() : ShipArrows(this), GeoViews(Bullets, Objects, Particles, Ships) {};
	PlaySpace(GameSurface* surface, const List<std::string>& arguments);
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
	ContainerSubrange< ViewBase< Ship*, float >, Ship* > findShips(Vec2F topLeft, Vec2F bottomRight);
public:
	Ship* Player;
	
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
	
	struct _GeoViews
	{
		template<typename T>
		using GeoView = GeometryView<T, BoundingRect, PhysicsObject, &PhysicsObject::Bounds>;
		
		GeoView<Bullet>         Bullets;
		GeoView<PhysicsObject*> Objects;
		GeoView<Particle>       Particles;
		GeoView<Ship*>					Ships;
		
		template<typename A, typename B, typename C, typename D>
		_GeoViews(A& a, B& b, C& c, D& d):Bullets(a),Objects(b),Particles(c),Ships(d){};
	} GeoViews;
	
	Map<Sector, Vec2I, &Sector::Position> Sectors;

	KeyframeList<Color> BackgroundGradient;
	
	Vec2F CameraPos;
	Vec2F ScreenSize;
	
	Vec2I Size;

	float AirDrag = 0.001f;
	float RotationAirDrag = 0.01f;
	
	Vec2F SectorSize = 25000;
	float SectorLookAhead = 15000;
	
	// User Interface
	Arrows ShipArrows;
	Label FrameRate;
	ShieldBar Shield;
	UIContainer GUIContainer;
	
	int GameFrame = 0;
	float GameTime = 0.0f;
	float LastDeltaTime = 0.0f;
	
	int ParticleBudget = 0;
	
	bool IsStressTesting = false;
};