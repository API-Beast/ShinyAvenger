//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "PlaySpace.h"
#include "AssetDefinition.h"
#include <Springbok/Graphics/SpriteRenderer.h>
#include <Springbok/Procedural/Noise.h>
#include <GL/gl.h>
#include <Springbok/Animation/Interpolation.h>
#include <Springbok/Utils/CliArgumentParser.h>
#include <Springbok/Audio/SoundInstance.h>

#include <iostream>

PlaySpace::PlaySpace(GameSurface* surface, const List<std::string>& arguments)
:
Renderer(surface),
GeoViews(Bullets, Objects, Particles, Ships)
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
	
	HDRTarget = new Framebuffer(surface->size().X, surface->size().Y, 16);
	
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
	
	// Play sounds
	Music = gAssets.MusicMainTheme->playGlobal();
	
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
	
	Music->Permanent = false;
	Music->pause();
}

void PlaySpace::checkSectorGeneration(Vec2F position)
{
	Vec2I sectCoordinates = position / SectorSize;

	auto& sector = Sectors[sectCoordinates];
	if(sector.Generated == false)
	{
		sector.Generated = true;
		NumberGeneratedSectors++;
		std::cout << "Generating sector at " << sectCoordinates << " / " << position << std::endl;
		
		Vec2F sectStart = sectCoordinates * SectorSize;
		Vec2F sectEnd = sectCoordinates * SectorSize + SectorSize;
		int primaryFaction = WorldRNG.getNumber(0, 2);
		int secondaryFaction = WorldRNG.getNumber(0, 2);
		while(primaryFaction == secondaryFaction)
			secondaryFaction = WorldRNG.getNumber(0, 2);
		
		int type = WorldRNG.getNumber(0, 2);
		
		// First sector is always a sector that is at war.
		if(NumberGeneratedSectors == 1)
			type = 1;
		
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
	GeoViews.Objects.YAxisView.update();
	GeoViews.Bullets.YAxisView.update();
	GeoViews.Particles.YAxisView.update();
	
	RenderTarget* oldTarget = Renderer.Context.renderTarget();
	
	if(oldTarget->size() != HDRTarget->size())
	{
		delete HDRTarget;
		HDRTarget = new Framebuffer(oldTarget->size().X, oldTarget->size().Y);
	}
	Renderer.Context.setRenderTarget(HDRTarget);
	Renderer.clear(Colors::Black);
	
	auto drawBG = [&](Image& img, Vec2F scale, Vec2F parral, Vec4F color = Colors::White)
	{
		Renderer.drawRepeatedInf(img, 0, scale, parral, color);
	};
	auto& r = Renderer;
	
	r.drawRepeatedInf(BackgroundStars, 0, 1.20f, 0.18f, Vec4F(Colors::White, 0.5f));
	r.drawRepeatedInf(BackgroundFogB,  0, 1.90f, 0.25f, Vec4F(0.22f, 0.15f, 0.14f, 0.5f));
	r.drawRepeatedInf(BackgroundStars, 0, 1.60f, 0.28f, Vec4F(Colors::White));
	r.drawRepeatedInf(BackgroundFog,   0, 1.75f, 0.30f, Vec3F(0.32f, 0.20f, 0.14f));
	
	for(GravitySource& src : GravitySources)
		src.draw(r);
	
	for(Bullet& obj : Bullets)
		obj.draw(r);
	for(PhysicsObject* obj : Objects)
		obj->draw(r);
	for(Particle& particle : Particles)
		particle.draw(r);
	
	for(GravitySource& src : GravitySources)
		src.drawTop(r);
	for(Bullet& obj : Bullets)
		obj.drawTop(r);
	
	r.flush();
	
	drawHUD();
	
	r.flush();
	Renderer.Context.setRenderTarget(oldTarget);
	Renderer.clear(Colors::Black);
	Renderer.Context.setShader(gAssets.ToneMapping);
	Renderer.drawRenderpass(*HDRTarget);
	Renderer.Context.setShader(ShaderProgram::GetDefaultShader());
}

void PlaySpace::drawHUD()
{
	
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
	
	if(!Player)
		TimeSincePlayerDestruction += time;
	
	GeoViews.update();
	
	SoundManager* manager = SoundManager::GetInstance();
	manager->setListenerPosition(CameraPos);
	
	if(IsStressTesting)
		time = 1.f / 30;
	
	if(!IsStressTesting)
		for(int i = 0; i < Systems.UsedLength; ++i)
		{
			Systems[i]->update(time, this);
		}
	
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
	
	
	for(int i = 0; i < Ships.UsedLength; ++i)
	{
		if(Ships[i]->canBeDespawned())
		{
			ObjectPointer<Ship>(Ships[i]).destroy(); // Object pointer lets other object pointers know that the object got deleted.
			Objects.quickRemove(Objects.findIndex(Ships[i]));
			Ships.quickRemove(i--);
		}
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
	
	GeoViews.update();
	
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
	
	GeoViews.update();
	
	for(Bullet& bullet : Bullets)
		for(Ship* ship : Ships)
			if(ship->Faction != bullet.Faction)
				if(ship->Status != Ship::Destroyed)
					if(IsIntersecting(bullet.Bounds, ship->Bounds))
						ship->onHit(&bullet, this);
		
	if(Player)
	{
		CameraPos = Player->Position;
		Renderer.Context.Camera.Position = CameraPos;
	
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
	}
	ParticleBudget = 0;
	
	if(Music->isFinished()){ Music->setOffset(0); Music->resume(); };
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
	if(Player)
	{
		if(down)
			Player->IsBraking = true;
		if(up);
		if(right)
			Player->Steering =  1.0f;
		if(left)
			Player->Steering = -1.0f;
	}
}

void PlaySpace::onActionInput(bool actionA, bool actionB, bool actionC)
{
	if(Player)
	{
		if(actionB)
			Player->IsShootingSecondary = true;
		if(actionA || actionC)
			Player->IsShooting = true;	
	}
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

void PlaySpace::castParticles(const Particle& def, Vec2F position, int amount, RangeF power, Color col, RangeF scale, RangeF lifetime, float turbulenceFactor)
{
	for(int i = 0; i < amount; ++i)
	{
		Noise1D turbulence;
		turbulence.Octaves = 2;
		turbulence.Interval = 0.25f;
		
		Particle p(def);
		float angleF = gRNG.getFloat();
		Angle angle = Angle::FromTurn(angleF);
		float angTurb = turbulence.calc(angleF);
		
		p.Colorization = col;
		p.Position = position;
		p.Size = gRNG.getNumber(scale);
		p.Speed = angle.toDirection() * power.interpolate(gRNG.getFloat()  * ((1-turbulenceFactor) + angTurb*angTurb*turbulenceFactor));
		p.Rotation = angle;
		p.LifeTimeMult = gRNG.getNumber(lifetime);
		spawnParticle(p, false);
	}
}

void PlaySpace::spawnParticle(Particle def, Vec2F position, float scale, Angle rotation, Color col, float lifetime, bool important)
{
	def.Position = position;
	def.Size = scale;
	def.Rotation = rotation;
	def.Colorization = col;
	def.LifeTimeMult = lifetime;
	spawnParticle(def, important);
}

void PlaySpace::spawnExplosion(Vec2F position, float size, float force, Color fireColor)
{
	RangeI amountParticles = RangeI(size/10, size/10+size/20);
	std::cout << amountParticles << " Particles will be spawned" << std::endl;
	//castParticles(gAssets.ExplosionCloud,          position, gRNG.getNumber(amountParticles), {0.f, force*2}, (fireColor+Colors::White)/2, {size/800, size/600}, {0.75f, 1.25f});
	castParticles(gAssets.ExplosionCloudAdditive,  position, gRNG.getNumber(amountParticles), {0.f, force*0.5f}, fireColor, {size/1000, size/700}, {0.75f, 1.25f});
	castParticles(gAssets.ExplosionSparks,         position, gRNG.getNumber(amountParticles), RangeF{force*1, force*3} + 10, fireColor);
	
	spawnParticle(gAssets.ExplosionFlash,          position, size/100, Angle::FromTurn(gRNG.getFloat()), fireColor, 1.f, true);
	spawnParticle(gAssets.ExplosionShockwave,      position, force/80, Angle::FromTurn(gRNG.getFloat()), fireColor, 1.f, true);
	spawnParticle(gAssets.GlowParticle,            position, size/13 , Angle::FromTurn(gRNG.getFloat()), fireColor, 4.f, true);

	gAssets.SoundExplosion02->play(position);
}
