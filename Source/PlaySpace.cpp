//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "PlaySpace.h"
#include <GL/gl.h>

PlaySpace::PlaySpace()
{
	ThePlayer = new Player;
	Objects.pushBack(ThePlayer);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
}

PlaySpace::~PlaySpace()
{
	for(PhysicsObject* obj : Objects)
		delete obj;
}

void PlaySpace::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	RenderContext r;
	r.CameraPos = {-400, -300};
	for(PhysicsObject* obj : Objects)
		obj->draw(r);
}

void PlaySpace::update(float time)
{
	ThePlayer->update(time);
	for(PhysicsObject* obj : Objects)
	{
		obj->Speed    += obj->Acceleration * time;
		obj->Speed    -= obj->Speed * (AirDrag * time) * obj->Speed.getLength();
		obj->Position += obj->Speed * time;
	}
}

void PlaySpace::onMovementInput(Angle angle)
{
	ThePlayer->MovementDirection = angle.toDirection();
}
