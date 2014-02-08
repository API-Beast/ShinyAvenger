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
	const int ENEMY_COUNT = 30;
	const int ENEMY_RANGE = 500;
	
	srand( time( NULL ) );
	
	for (int i = 0; i < ENEMY_COUNT; ++i)
	{
		const double ShipSpeed = rand() % 100 + 550.0;
		EnemyBehavior *behavior = new TrackingBehavior(ThePlayer, ShipSpeed);
		Enemy *enemy = new Enemy(behavior);
		enemy->Position.X = rand() % 4000 - 2000;
		enemy->Position.Y = rand() % 4000 - 2000;
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
	r.CameraPos = CameraPos;
	
	for(GravitySource& src : GravitySources)
		src.draw(r);
	
	r.setColor(Colors::White);
	for(PhysicsObject* obj : Objects)
		obj->draw(r);
	for(Bullet& obj : PlayerBullets)
		obj.draw(r);
}

void PlaySpace::update(float time)
{
	GameTime += time;
	
	for(PhysicsObject* obj : Objects)
		obj->update(time, this);
	for(GravitySource& src : GravitySources)
		src.update(time, this);
	for(Bullet& obj : PlayerBullets)
		obj.update(time, this);
	
	// Physics
	for(PhysicsObject* obj : Objects)
		applyPhysics(obj, time);
	for(Bullet& obj : PlayerBullets)
		applyPhysics(&obj, time);
	
	CameraPos = Vec2F(-400, -300) + ThePlayer->Position;
	
	// Will be reset to true before next PlaySpace::update
	ThePlayer->IsShooting = false;
}

void PlaySpace::applyPhysics(PhysicsObject* obj, float dt)
{
	obj->Speed += obj->Acceleration * dt;
	obj->Speed -= obj->Speed * (AirDrag * obj->Drag * dt) * obj->Speed.getLength();
	
	// Add gravity
	for(GravitySource& src : GravitySources)
		src.influence(obj, dt);
	
	obj->Position += obj->Speed * dt;
}

void PlaySpace::onMovementInput(Angle angle, float time)
{
	ThePlayer->TargetDirection = Approach(ThePlayer->TargetDirection, angle, time);
	ThePlayer->AcclerateFactor = 1.0f;
}

void PlaySpace::onMouseHoldInput(Vec2F mousePos)
{
	ThePlayer->ShootingDirection = ((mousePos + CameraPos) - ThePlayer->Position).getAngle();
	ThePlayer->IsShooting = true;
}

void PlaySpace::spawnPlayerBullet(Bullet bullet)
{
	PlayerBullets.pushBack(bullet);
}
