//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Player.h"
#include "Space.h"
#include "PhysicsObject.h"
#include <Springbok/Geometry/Vec2.h>
#include <Springbok/Containers/List.h>

class PlaySpace : public Space
{
public:
	PlaySpace();
	virtual ~PlaySpace();
	virtual void update(float time);
	virtual void draw();
	void onMovementInput(Angle angle);
public:
	Player* ThePlayer;
	List<PhysicsObject*> Objects;
	float AirDrag = 0.01;
};