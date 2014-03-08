//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "PlaySpace.h"
#include "AssetDefinition.h"
#include <GL/gl.h>
#include <Springbok/Animation/Interpolation.h>
#include <Springbok/Utils/CliArgumentParser.h>

#include <iostream>

PlaySpace::PlaySpace(GameSurface* surface, const List<std::string>& arguments) : ShipArrows(this), GeoViews(Bullets, Objects, Particles, Ships)
{
	gAssets.initAll();
	
	CliArgumentParser argParser;
	argParser.addSwitch("--stress-test");
	
	argParser.parse(arguments);
	IsStressTesting = argParser["--stress-test"].IsSet;

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
	
	checkSectorGeneration(0);
	HomeSystem = Systems.front();
	
	Player = HomeSystem->spawnShip(this);
	Player->Sprite = Image("Player/Ship.png");
	Player->FactionColorSprite = Image("Player/ShipFactionColor.png");
	Player->PrimaryWeapon = gAssets.MiniGun;
	Player->SecondaryWeapon = gAssets.MissileLauncher;
	Player->ImpulseColor = RGB(0.4f, 0.8f, 1.0f);
	Player->AI = NULL;

	// User Interface
	GUIContainer.append(&ShipArrows);
	GUIContainer.append(&FrameRate);
	//FrameRate.Font = &gAssets.BigFont;
	Shield.Target = Player;
	Shield.Position.X = 15;
	Shield.Position.Y = 25;
	GUIContainer.append(&Shield);
	
	if(IsStressTesting)
	{
		WorldRNG.LowSeed = 453812932;
		WorldRNG.HighSeed = 354843;
		for(float y = -25000; y < 25000; y += 5000)
			for(float x = -25000; x < 25000; x += 5000)
				checkSectorGeneration({x, y});
			
		for(SolarSystem* system : Systems)
			system->spawnGroup(system->getPosition(), 8, this);
		
		std::cout << "Stress testing: " << Ships.UsedLength << " Ships, " << Systems.UsedLength << " Solar Systems" << std::endl;
	}
}

PlaySpace::~PlaySpace()
{
	std::cout << "We simulated " << GameFrame << " Frames in " << GameTime << " seconds" << std::endl;
	std::cout << "We ended up with " << Objects.UsedLength << " Objects, " << Systems.UsedLength << " Systems and " << Sectors.Data.UsedLength << " Sectors" << std::endl;
	
	for(PhysicsObject* obj : Objects)
		delete obj;
	
	for(SolarSystem* sec : Systems)
		delete sec;
	
	gAssets.disposeAll();
}

void PlaySpace::checkSectorGeneration(Vec2F position)
{
	Vec2I sectCoordinates = position / SectorSize;

	auto& sector = Sectors[sectCoordinates];
	if(sector.Generated == false)
	{
		sector.Generated = true;
		std::cout << "Generating sector at " << sectCoordinates << " / " << position << std::endl;
		
		Vec2F sectStart = sectCoordinates * SectorSize;
		Vec2F sectEnd = sectCoordinates * SectorSize + SectorSize;
		int primaryFaction = WorldRNG.getNumber(0, 2);
		int secondaryFaction = WorldRNG.getNumber(0, 2);
		while(primaryFaction == secondaryFaction)
			secondaryFaction = WorldRNG.getNumber(0, 2);
		int type = WorldRNG.getNumber(0, 2);
		
		if(type == 0)
		{
			generateSystem(WorldRNG.getVec2(sectStart, sectEnd), primaryFaction);
		}
		else if(type == 1)
		{
			generateSystem(WorldRNG.getVec2(sectStart, sectEnd), primaryFaction);
			generateSystem(WorldRNG.getVec2(sectStart, sectEnd), secondaryFaction);
		}
		else if(type == 2)
		{
			generateSystem(WorldRNG.getVec2(sectStart, sectEnd), primaryFaction);
			generateSystem(WorldRNG.getVec2(sectStart, sectEnd), primaryFaction);
			generateSystem(WorldRNG.getVec2(sectStart, sectEnd), secondaryFaction);
		}
	}
}

SolarSystem* PlaySpace::generateSystem(Vec2F position, int faction)
{
	SolarSystem* sect = new SolarSystem(position, WorldRNG.getNumber(3500, 4500), this, faction);
	std::cout << "Generating solar system at " << position << std::endl;
	sect->Prototype.PrimaryWeapon = gAssets.Phaser;
	Systems.pushBack(sect);
	return sect;
}

void PlaySpace::draw()
{
	//Color bgColor = BackgroundGradient[Player->Position.getLength()];
	//glClearColor(bgColor.Red, bgColor.Green, bgColor.Blue, 1.f);
	glClearColor(0, 0, 0, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	GeoViews.Objects.YAxisView.update();
	GeoViews.Bullets.YAxisView.update();
	GeoViews.Particles.YAxisView.update();
	
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
		rBG.setColor(BackgroundGradient[Player->Position.length()+4000]* 0.6f);
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
		rBG.setColor(BackgroundGradient[Player->Position.length()]* 0.9f);
		BackgroundFog.drawRepeated(rBG);
	}
	
	for(GravitySource& src : GravitySources)
		src.draw(r);
	
	float start = CameraPos.Y - 600;
	float end = start + ScreenSize.Y + 600;
	
	r.setColor(Colors::White);
	// Draw clipped
	/*for(Bullet& obj : GeoViews.Bullets.YAxisView.getRange(start, end))
		obj.draw(r);
	for(PhysicsObject* obj : GeoViews.Objects.YAxisView.getRange(start, end))
		obj->draw(r);
	for(Particle& particle : GeoViews.Particles.YAxisView.getRange(start, end))
		particle.draw(r);*/
	
	// Draw unclipped
	for(Bullet& obj : Bullets)
		obj.draw(r);
	for(PhysicsObject* obj : Objects)
		obj->draw(r);
	for(Particle& particle : Particles)
		particle.draw(r);
	
	/*{
		RenderContext rBG(r);
		rBG.Parallaxity = 2.00f;
		rBG.Scale = 2.f;
		rBG.setColor(BackgroundGradient[Player->Position.getLength()+2000]* 0.9f);
		ForegroundFog.drawRepeated(rBG);
	}*/
	
	for(GravitySource& src : GravitySources)
		src.drawTop(r);
	for(Bullet& obj : Bullets)
		obj.drawTop(r);
	
	RenderContext gui;	
	GUIContainer.render(&gui);
}

void PlaySpace::update(float time)
{
	if(time > 0.25f)
	{
		ParticleBudget = 0;
		time = 0.25f;
	}
	else
		ParticleBudget = 500;
	
	LastDeltaTime = time;
	GameTime += time;
	GameFrame += 1;
	
	if(IsStressTesting)
		time = 1.f / 30;
	
	if(!IsStressTesting)
		for(int i = 0; i < Systems.UsedLength; ++i)
		{
			Systems[i]->update(time, this);
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
	
	GeoViews.Objects.XAxisView.update();
	GeoViews.Bullets.XAxisView.update();
	GeoViews.Particles.XAxisView.update();
	
	// Add gravity
	for(GravitySource& src : GravitySources)
	{
		float start = src.Position.X - src.Range*2;
		float end = src.Position.X + src.Range*2;
		
		for(auto* obj : GeoViews.Objects.XAxisView.getRange(start, end))
			src.influence(obj, time);
		for(auto& obj : GeoViews.Bullets.XAxisView.getRange(start, end))
			src.influence(&obj, time);
		for(auto& obj : GeoViews.Particles.XAxisView.getRange(start, end))
			src.influence(&obj, time);
	}
	
	GeoViews.Bullets.XAxisView.update();
	GeoViews.Ships.XAxisView.update();
	
	for(Bullet& bullet : Bullets)
		for(Ship* ship : Ships)
			if(ship->Faction != bullet.Faction)
				if(ship->Status != Ship::Destroyed)
					if(IsIntersecting(bullet.Bounds, ship->Bounds))
						ship->onHit(&bullet, this);
		
	CameraPos = -(ScreenSize/2) + Player->Position;
	
	// Will be reset to true before next PlaySpace::update
	Player->IsShooting = false;
	Player->IsShootingSecondary = false;
	Player->IsBraking = false;
	Player->IsStabilizing = false;
	Player->Steering = 0.0f;
	
	if(!IsStressTesting)
	{
		checkSectorGeneration(Player->Position);
		checkSectorGeneration(Player->Position + Vec2F(SectorLookAhead, 0));
		checkSectorGeneration(Player->Position + Vec2F(0, SectorLookAhead));
		checkSectorGeneration(Player->Position - Vec2F(SectorLookAhead, 0));
		checkSectorGeneration(Player->Position - Vec2F(0, SectorLookAhead));
		checkSectorGeneration(Player->Position + Vec2F(SectorLookAhead));
		checkSectorGeneration(Player->Position - Vec2F(SectorLookAhead));
		
		checkSectorGeneration(Player->Position + Vec2F(SectorLookAhead/2, 0));
		checkSectorGeneration(Player->Position + Vec2F(0, SectorLookAhead/2));
		checkSectorGeneration(Player->Position - Vec2F(SectorLookAhead/2, 0));
		checkSectorGeneration(Player->Position - Vec2F(0, SectorLookAhead/2));
		checkSectorGeneration(Player->Position + Vec2F(SectorLookAhead/2));
		checkSectorGeneration(Player->Position - Vec2F(SectorLookAhead/2));
	}
	
	FrameRate.Text = std::to_string(LastDeltaTime*1000).substr(0, 4);
	ParticleBudget = 0;
}

void PlaySpace::applyPhysics(PhysicsObject* obj, float dt)
{
	obj->Speed += obj->Acceleration * dt;
	obj->Speed -= obj->Speed * ((AirDrag * obj->Drag * dt) + (obj->NegativeForce * dt)) * obj->Speed.length();
	
	obj->Rotation += Angle(obj->RotationSpeed * dt);
	
	Angle diff = Angle(obj->Speed) - obj->Rotation;
	obj->Rotation += diff * obj->Flow * (obj->Speed.length() / 500) * dt;
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
	if(actionB)
		Player->IsShootingSecondary = true;
	if(actionA || actionC)
		Player->IsShooting = true;
}

void PlaySpace::onMouseHoldInput(Vec2F mousePos)
{
}

void PlaySpace::spawnPlayerBullet(Bullet bullet)
{
	Bullets.pushBack(bullet);
}

void PlaySpace::spawnParticle(Particle particle, bool important)
{
	if(ParticleBudget > 0 || important)
		if((CameraPos - particle.Position).length() < 4000)
		{
			Particles.pushBack(particle);
			ParticleBudget--;
		}
}

bool PlaySpace::isHostile(Ship *shipA, Ship *shipB)
{
   return shipA->Faction != shipB->Faction;
}

bool PlaySpace::isHostile(int a, int b)
{
   return a != b;
}

Color PlaySpace::getFactionColor(int factionID)
{
	static const Color FactionColors[3] = {Palette::Vibrant::Lime, Palette::Vibrant::Lilac, Palette::Vibrant::Orange};
	return FactionColors[factionID];
}

ContainerSubrange<ViewBase< Ship*, float >, Ship*> PlaySpace::findShips(Vec2F topLeft, Vec2F bottomRight)
{
	return GeoViews.Ships.XAxisView.getRange(topLeft.X, bottomRight.X);
}
