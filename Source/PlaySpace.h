//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Player.h"
#include "Space.h"
#include "Enemy.h"
#include "PhysicsObject.h"
#include "GravitySource.h"

#include <Springbok/Geometry/Vec2.h>
#include <Springbok/Containers/List.h>

class PlaySpace : public Space
{
public:
	PlaySpace();
	virtual ~PlaySpace();
	virtual void update(float time);
	virtual void draw();
	void onMovementInput(Angle angle, float time);
public:
	Player* ThePlayer;
	List<PhysicsObject*> Objects;
	List<Enemy*> Enemies;
	List<GravitySource> GravitySources;

	float AirDrag = 0.0005f;
	float GameTime = 0.0f;
};