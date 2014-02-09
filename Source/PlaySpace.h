//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Player.h"
#include "Space.h"
#include "Enemy.h"
#include "EnemyBehavior.h"
#include "EnemySpawner.h"
#include "PhysicsObject.h"
#include "GravitySource.h"
#include "Bullet.h"
#include "UI.h"

#include <Springbok/Geometry/Vec2.h>
#include <Springbok/Containers/List.h>
#include <Springbok/Platform/GameSurface.h>

class PlaySpace : public Space
{
public:
	PlaySpace(GameSurface* surface);
	virtual ~PlaySpace();
	virtual void update(float time);
	virtual void draw();
	void applyPhysics(PhysicsObject* obj, float dt);
	void onMovementInput(Angle angle, float time);
	void onMouseHoldInput(Vec2F mousePos);
	void spawnPlayerBullet(Bullet bullet);
public:
	Player* ThePlayer;
	List<PhysicsObject*> Objects;
	List<Enemy*> Enemies;
	List<Bullet> PlayerBullets;
	List<GravitySource> GravitySources;
	KeyframeList<Color> BackgroundGradient;
	EnemySpawner Spawner;
	UIContainer GUIContainer;
	
	Vec2F CameraPos;
	Vec2F ScreenSize;
	
	Vec2I Size;

	float AirDrag = 0.0005f;
	float GameTime = 0.0f;
};