//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "PlaySpace.h"
#include <GL/gl.h>
#include <Springbok/Animation/Interpolation.h>

PlaySpace::PlaySpace(GameSurface* surface)
{
	ThePlayer = new Player;
	ThePlayer->Position = {200, 150};
	Objects.pushBack(ThePlayer);
	
	srand( time( NULL ) );
	
	GravitySources.pushBack({Vec2F(0,0), 200.f, 1500.f, ColorRGB(0.62f, 0.2f, 0.44f), ColorRGB(0.92f, 0.5f, 0.44f)});
	
	BackgroundGradient[0].insert(Color(0.22f, 0.15f, 0.24f));
	BackgroundGradient[2000].insert(Color(0.42f, 0.15f, 0.14f));
	BackgroundGradient[5000].insert(Color(0.12f, 0.10f, 0.10f));
	BackgroundGradient[9000].insert(Colors::Black);
	
	ScreenSize = surface->getSize();
	Size = Vec2I(5000, 5000);
	
	EnemyArrow *Arrow = new EnemyArrow(this);
	GUIContainer.append(Arrow);
}

PlaySpace::~PlaySpace()
{
	for(PhysicsObject* obj : Objects)
		delete obj;
}

void PlaySpace::draw()
{
	Color bgColor = BackgroundGradient[ThePlayer->Position.getLength()];
	glClearColor(bgColor.Red, bgColor.Green, bgColor.Blue, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	RenderContext r;
	r.CameraPos = CameraPos;
	
	for(GravitySource& src : GravitySources)
		src.draw(r);
	
	r.setColor(Colors::White);
	for(Bullet& obj : PlayerBullets)
		obj.draw(r);
	for(PhysicsObject* obj : Objects)
		obj->draw(r);
	for(Particle& particle : Particles)
		particle.draw(r);
	
	GUIContainer.draw(&r);
}

void PlaySpace::update(float time)
{
	GameTime += time;
	
	Spawner.update(time, this);
	GUIContainer.update(time);
	
	for(int i = 0; i < PlayerBullets.UsedLength; ++i)
	{
		if(PlayerBullets[i].canBeDespawned())
			PlayerBullets.quickRemove(i--);
	}
	
	for(int i = 0; i < Particles.UsedLength; ++i)
	{
		if(Particles[i].canBeDespawned())
			Particles.quickRemove(i--);
	}
	
	
	for(PhysicsObject* obj : Objects)
		obj->update(time, this);
	for(GravitySource& src : GravitySources)
		src.update(time, this);
	for(Bullet& obj : PlayerBullets)
		obj.update(time, this);
	for(Particle& particle : Particles)
		particle.update(time, this);
	
	// Physics
	for(PhysicsObject* obj : Objects)
		applyPhysics(obj, time);
	for(Bullet& obj : PlayerBullets)
		applyPhysics(&obj, time);
	for(Particle& particle : Particles)
		applyPhysics(&particle, time);
	
	CameraPos = -(ScreenSize/2) + ThePlayer->Position;
	
	// Will be reset to true before next PlaySpace::update
	ThePlayer->IsShooting = false;
	ThePlayer->IsBraking = false;
	ThePlayer->IsStabilizing = false;
	ThePlayer->Steering = 0.0f;
}

void PlaySpace::applyPhysics(PhysicsObject* obj, float dt)
{
	obj->Speed += obj->Acceleration * dt;
	obj->Speed -= obj->Speed * (AirDrag * obj->Drag * dt) * obj->Speed.getLength();
	
	// Add gravity
	for(GravitySource& src : GravitySources)
		src.influence(obj, dt);
	
	obj->Rotation += obj->RotationSpeed * dt;
	
	float diff = obj->Rotation.difference(obj->Speed.getAngle());
	obj->Rotation += MinAbs(diff * obj->Flow * (obj->Speed.getLength() / 400 + obj->Stabilizer) * dt, diff);
	obj->RotationSpeed -= MinAbs(((obj->Stabilizer * dt) + (AirDrag * obj->Flow * dt)) * obj->RotationSpeed, obj->RotationSpeed);
	
	obj->Position += obj->Speed * dt;
}

void PlaySpace::onMovementInput(bool up, bool down, bool right, bool left, float time)
{
	if(down)
		ThePlayer->IsBraking = true;
	if(up)
		ThePlayer->IsStabilizing = true;
	if(right)
		ThePlayer->Steering =  1.0f;
	if(left)
		ThePlayer->Steering = -1.0f;
}

void PlaySpace::onActionInput(bool actionA, bool actionB, bool actionC)
{
	ThePlayer->IsShooting = true;
}

void PlaySpace::onMouseHoldInput(Vec2F mousePos)
{
}

void PlaySpace::spawnPlayerBullet(Bullet bullet)
{
	PlayerBullets.pushBack(bullet);
}

void PlaySpace::spawnParticle(Particle particle)
{
	Particles.pushBack(particle);
}
