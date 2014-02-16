//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Space.h"
#include "ShipSpawner.h"
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
public:
	Ship* Player;
	
	Image BackgroundStars;
	Image BackgroundFog;
	Image BackgroundFogB;
	Image ForegroundFog;
	
	List<PhysicsObject*> Objects;
	List<Bullet> Bullets;
	List<GravitySource> GravitySources;
	List<Particle> Particles;
	List<Ship*> Ships;
	KeyframeList<Color> BackgroundGradient;
	ShipSpawner Spawner;
	UIContainer GUIContainer;
	
	Vec2F CameraPos;
	Vec2F ScreenSize;
	
	Vec2I Size;

	float AirDrag = 0.0005f;
	float GameTime = 0.0f;
};