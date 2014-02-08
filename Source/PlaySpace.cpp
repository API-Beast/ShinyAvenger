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
	
	// Add enemies
	const int ENEMY_COUNT = 40;
	const int ENEMY_RANGE = 500;
	
	srand( time( NULL ) );
	
	for (int i = 0; i < ENEMY_COUNT; ++i) {
	  
	    const double Speed = rand() % 10 + 80;
	    EnemyBehavior *behavior = new TrackingBehavior(ThePlayer, Speed);
	    Enemy *enemy = new Enemy(behavior);
	    enemy->Position.X = rand() % 2000 - 1000;
	    enemy->Position.Y = rand() % 2000 - 1000;
	    Enemies.pushBack(enemy);
	    Objects.pushBack(enemy);
	}
	
	GravitySources.pushBack({Vec2F(0,0), 0.3f, 750.f, ColorRGB(0.62f, 0.2f, 0.44f), ColorRGB(0.92f, 0.5f, 0.44f)});
	
	glClearColor(0.22f, 0.15f, 0.24f, 0.0f);
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
	
	for(GravitySource& src : GravitySources)
		src.draw(r);
	
	r.setColor(Colors::White);
	for(PhysicsObject* obj : Objects)
		obj->draw(r);
}

void PlaySpace::update(float time)
{
	GameTime += time;
	
	for(PhysicsObject* obj : Objects)
		obj->update(time);
	for(GravitySource& src : GravitySources)
		src.update(time);
	
	// Physics
	for(PhysicsObject* obj : Objects)
	{
		obj->Speed    += obj->Acceleration * time;
		obj->Speed    -= obj->Speed * (AirDrag * obj->Drag * time) * obj->Speed.getLength();
		
		// Add gravity
		for(GravitySource& src : GravitySources)
			src.influence(obj, time);
		
		obj->Position += obj->Speed * time;
	}
}

void PlaySpace::onMovementInput(Angle angle, float time)
{
	ThePlayer->TargetDirection = Approach(ThePlayer->TargetDirection, angle, time);
	ThePlayer->AcclerateFactor = 1.0f;
}

