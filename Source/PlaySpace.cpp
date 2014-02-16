//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "PlaySpace.h"
#include <GL/gl.h>
#include <Springbok/Animation/Interpolation.h>

#include <iostream>

PlaySpace::PlaySpace(GameSurface* surface)
{
	BackgroundStars = Image("BackgroundStars.png");
	BackgroundFog = Image("BackgroundFog.png");
	BackgroundFogB = Image("BackgroundFogB.png");
	
	Player = new Ship(Image("Player/Sprite.png"));
	Player->Position = Vec2F{200, 150};
	Player->Faction = 0;
	Ships.pushBack(Player);
	
	Bullet& b = Player->Weapon.BulletPrototype;
	b.Mass = 5;
	b.Drag = 0;
	b.Flow = 5;
	b.Lifetime = 1.f;

	b.ColorAnimation.insert(0.5, Colors::Saturated::Orange);
	b.ColorAnimation.insert(1.0, Colors::Saturated::Red);

	b.GlowColorAnimation = Colors::Saturated::Yellow;
	b.GlowColorAnimation.insert(0.5, Colors::Saturated::Orange);
	b.GlowColorAnimation.insert(1.0, Colors::Saturated::Red);

	b.AlphaAnimation.insert(0.8, 1.f);
	b.AlphaAnimation.insert(1.0, 0.f);

	b.ScaleAnimation.insert(0.2, {1.f, 4.f});
	b.ScaleAnimation.insert(0.8, {1.5f, 3.f});
	b.ScaleAnimation.insert(1.0, {0.5f, 4.f});

	b.GlowScaleAnimation = Vec2F(2.f);

	b.Sprite = Image("Player/Bullet.png");
	b.Glow = Image("Glow.png");
	
	Spawner.Prototype.Sprite = Image("Enemy/Enemy01.png");
	Spawner.Prototype.Weapon.BulletPrototype = Player->Weapon.BulletPrototype;
	Spawner.Prototype.Faction = 2;
	Spawner.spawnShip(this, Vec2F(500, 500));
	
	Objects.pushBack(Player);
	
	GravitySources.pushBack({Vec2F(0,0), 100.f, 2500.f, ColorRGB(0.62f, 0.2f, 0.44f), ColorRGB(0.92f, 0.5f, 0.44f)});
	
	BackgroundGradient.insert(0, Color(0.22f, 0.15f, 0.24f));
	BackgroundGradient.insert(10000, Color(0.42f, 0.15f, 0.14f));
	BackgroundGradient.insert(20000, Color(0.32f, 0.10f, 0.24f));
	BackgroundGradient.insert(30000, Color(0.12f, 0.10f, 0.10f));
	BackgroundGradient.insert(35000, Colors::Black);
	
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
	//Color bgColor = BackgroundGradient[Player->Position.getLength()];
	//glClearColor(bgColor.Red, bgColor.Green, bgColor.Blue, 1.f);
	glClearColor(0, 0, 0, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	RenderContext r;
	r.RenderTargetOrigin = 0;
	r.RenderTargetSize = ScreenSize;
	r.CameraPos = CameraPos;
	r.setColor(Colors::White);
	
	{
		RenderContext rBG(r);
		rBG.Parallaxity = 0.21f;
		rBG.Scale = 1.2f;
		//rBG.setColor(BackgroundGradient[Player->Position.getLength()+2500]* 0.8f);
		BackgroundStars.drawRepeated(rBG);
	}
	
	{
		RenderContext rBG(r);
		rBG.Parallaxity = 0.25f;
		rBG.Scale = 1.9f;
		rBG.setColor(BackgroundGradient[Player->Position.getLength()+3000]* 0.6f);
		BackgroundFogB.drawRepeated(rBG);
	}
	
	{
		RenderContext rBG(r);
		rBG.Parallaxity = 0.28f;
		rBG.Scale = 1.6f;
		//rBG.setColor(BackgroundGradient[Player->Position.getLength()+2500]* 0.8f);
		BackgroundStars.drawRepeated(rBG);
	}
	
	{
		RenderContext rBG(r);
		rBG.Parallaxity = 0.35f;
		rBG.Scale = 3.f;
		rBG.setColor(BackgroundGradient[Player->Position.getLength()+4500]* 0.9f);
		BackgroundFog.drawRepeated(rBG);
	}
	
	for(GravitySource& src : GravitySources)
		src.draw(r);
	
	r.setColor(Colors::White);
	for(Bullet& obj : Bullets)
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
	
	for(int i = 0; i < Bullets.UsedLength; ++i)
	{
		if(Bullets[i].canBeDespawned())
			Bullets.quickRemove(i--);
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
	for(Bullet& obj : Bullets)
		obj.update(time, this);
	for(Particle& particle : Particles)
		particle.update(time, this);
	
	// Physics
	for(PhysicsObject* obj : Objects)
		applyPhysics(obj, time);
	for(Bullet& obj : Bullets)
		applyPhysics(&obj, time);
	for(Particle& particle : Particles)
		applyPhysics(&particle, time);
	
	for(Bullet& bullet : Bullets)
		for(Ship* ship : Ships)
			if(ship->Faction != bullet.Faction)
				if(IsIntersecting(bullet.Bounds, ship->Bounds))
				{
					bullet.onHit(ship, this);
					ship->onHit(&bullet, this);
				}
	
	CameraPos = -(ScreenSize/2) + Player->Position;
	
	// Will be reset to true before next PlaySpace::update
	Player->IsShooting = false;
	Player->IsBraking = false;
	Player->IsStabilizing = false;
	Player->Steering = 0.0f;
}

void PlaySpace::applyPhysics(PhysicsObject* obj, float dt)
{
	obj->Speed += obj->Acceleration * dt;
	obj->Speed -= obj->Speed * ((AirDrag * obj->Drag * dt) + (obj->NegativeForce * dt)) * obj->Speed.getLength();
	
	// Add gravity
	for(GravitySource& src : GravitySources)
		src.influence(obj, dt);
	
	obj->Rotation += Angle(obj->RotationSpeed * dt);
	
	Angle diff = Angle(obj->Speed) - obj->Rotation;
	obj->Rotation += diff * obj->Flow * (obj->Speed.getLength() / 500) * dt;
	obj->RotationSpeed -= ((obj->Stabilizer * dt) + (AirDrag * obj->Flow * dt)) * obj->RotationSpeed;

	obj->Position += obj->Speed * dt;
	obj->updateBounds();
}

void PlaySpace::onMovementInput(bool up, bool down, bool right, bool left, float time)
{
	if(down)
		Player->IsBraking = true;
	if(up);
	if(right)
		Player->Steering =  1.0f;
	if(left)
		Player->Steering = -1.0f;
}

void PlaySpace::onActionInput(bool actionA, bool actionB, bool actionC)
{
	Player->IsShooting = true;
}

void PlaySpace::onMouseHoldInput(Vec2F mousePos)
{
}

void PlaySpace::spawnPlayerBullet(Bullet bullet)
{
	Bullets.pushBack(bullet);
}

void PlaySpace::spawnParticle(Particle particle)
{
	Particles.pushBack(particle);
}
