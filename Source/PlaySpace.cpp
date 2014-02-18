//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "PlaySpace.h"
#include "AssetDefinition.h"
#include <GL/gl.h>
#include <Springbok/Animation/Interpolation.h>

PlaySpace::PlaySpace(GameSurface* surface)
{
	gAssets.initAll();
	
	BigFont.loadRange(Image("UI/BigNumbers.png"), '\'', ':');
	SmallFont.loadGrid(Image("UI/SmallFont.png"), 0, 16);

	BackgroundStars = Image("BackgroundStars.png");
	BackgroundFog = Image("BackgroundFog.png");
	BackgroundFogB = Image("BackgroundFogB.png");
	ForegroundFog = Image("ForegroundFog.png");
	
	BackgroundGradient.insert(0, Color(0.22f, 0.15f, 0.24f));
	BackgroundGradient.insert(10000, Color(0.42f, 0.15f, 0.14f));
	BackgroundGradient.insert(20000, Color(0.32f, 0.10f, 0.24f));
	BackgroundGradient.insert(30000, Color(0.12f, 0.10f, 0.10f));
	BackgroundGradient.insert(35000, Colors::Black);
	
	ScreenSize = surface->getSize();
	Size = Vec2I(5000, 5000);
	
	HomeSector = generateSector(0);
	generateSector({-2000.f, -500.f});
	generateSector({2000.f, 1000.f});
	
	Player = HomeSector->spawnShip(Vec2F(0.f, 0.f), this);
	Player->PrimaryWeapon = gAssets.MiniGun;
	Player->AI = NULL;
	Player->Position = Vec2F{200, 150};

	EnemyArrow *Arrow = new EnemyArrow(this);
	GUIContainer.append(Arrow);
}

PlaySpace::~PlaySpace()
{
	for(PhysicsObject* obj : Objects)
		delete obj;
	
	for(Sector* sec : Sectors)
		delete sec;
}

Sector* PlaySpace::generateSector(Vec2F position)
{
	Sector* sect = new Sector(position, WorldRNG.generate(500, 1000), this);
	sect->Prototype.PrimaryWeapon = gAssets.Phaser;
	Sectors.pushBack(sect);
	return sect;
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
		rBG.setColor(BackgroundGradient[Player->Position.getLength()+4000]* 0.6f);
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
		rBG.setColor(BackgroundGradient[Player->Position.getLength()]* 0.9f);
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
	
	{
		RenderContext rBG(r);
		rBG.Parallaxity = 2.00f;
		rBG.Scale = 2.f;
		rBG.setColor(BackgroundGradient[Player->Position.getLength()+2000]* 0.9f);
		ForegroundFog.drawRepeated(rBG);
	}
	
	GUIContainer.draw(&r);
	
	RenderContext gui;
	gui.Alignment = 0;
	gui.setColor(Colors::White);
	BigFont.draw<char>(std::to_string(LastDeltaTime*1000).substr(0, 4), gui);
	//gui.Offset = {0, 200};
	//SmallFont.draw<char>("Test!! 123xyz", gui);
}

void PlaySpace::update(float time)
{
	LastDeltaTime = time;
	GameTime += time;
	
	for(int i = 0; i < Sectors.UsedLength; ++i)
	{
		Sectors[i]->update(time, this);
	}
	
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
	
	// The more particles exist the faster time passes for them, the faster they die
	float particleTimeFactor = Max(float(Particles.UsedLength) / SoftMaxParticleCount, 1.0f);
	for(Particle& particle : Particles)
		particle.update(time * particleTimeFactor, this);
	
	// Physics
	for(PhysicsObject* obj : Objects)
		applyPhysics(obj, time);
	for(Bullet& obj : Bullets)
		applyPhysics(&obj, time);
	for(Particle& particle : Particles)
		applyPhysics(&particle, time * particleTimeFactor);
	
	for(Bullet& bullet : Bullets)
		for(Ship* ship : Ships)
			if(ship->Faction != bullet.Faction)
				if(ship->Status != Ship::Destroyed)
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
	obj->RotationSpeed -= ((obj->Stabilizer * dt) + (RotationAirDrag * obj->Flow * dt)) * obj->RotationSpeed;

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
	if((CameraPos - particle.Position).getLength() < 4000)
		Particles.pushBack(particle);
}

bool PlaySpace::isHostile(Ship *shipA, Ship *shipB)
{
   return shipA->Faction != shipB->Faction;
}

ColorRGB PlaySpace::getFactionColor(int factionID)
{
	RandomNumberGenerator rng(factionID);
  ColorHSY hsy(rng.generate(), rng.generate(0.8f, 1.0f), rng.generate(0.6f, 1.2f));
	return ColorRGB(hsy);
}
