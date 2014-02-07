//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "PlaySpace.h"
#include <GL/gl.h>
#include <Springbok/Animation/Interpolation.h>

PlaySpace::PlaySpace()
{
	ThePlayer = new Player;
	ThePlayer->Position = {200, 150};
	Objects.pushBack(ThePlayer);
	
	GravitySources.pushBack({Vec2F(0,0), 2.f, 100.f, Image("GravitySource.png")});
	GravitySources.pushBack({Vec2F(600,800), 2.f, 100.f, Image("GravitySource.png")});
	GravitySources.pushBack({Vec2F(400,400), -2.f, 50.f, Image("AntiGravitySource.png")});
	
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
	r.CameraPos = Vec2F(-400, -300) + ThePlayer->Position;
	for(PhysicsObject* obj : Objects)
		obj->draw(r);
	for(GravitySource& src : GravitySources)
	{
		r.Offset = src.Position;
		src.Graphics.draw(r);
	}
}

void PlaySpace::update(float time)
{
	ThePlayer->update(time);
	for(PhysicsObject* obj : Objects)
	{
		obj->Speed    += obj->Acceleration * time;
		obj->Speed    -= obj->Speed * (AirDrag * obj->Drag * time) * obj->Speed.getLength();
		
		// Add gravity
		for(GravitySource& src : GravitySources)
		{
			Vec2F difference = (src.Position - obj->Position);
			float distance = difference.getLength();
			Vec2F direction = difference.normalized();
			obj->Speed += (direction * src.Gravity * obj->Mass) / Max((distance/src.Range)*(distance/src.Range),1.0f);
		}
		
		obj->Position += obj->Speed * time;
	}
}

void PlaySpace::onMovementInput(Angle angle, float time)
{
	ThePlayer->TargetDirection = Approach(ThePlayer->TargetDirection, angle, time);
	ThePlayer->AcclerateFactor = 1.0f;
}
