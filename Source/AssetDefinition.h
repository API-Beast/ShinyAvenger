//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Bullet.h"
#include "Particle.h"
#include "Ship.h"

#include <Springbok/Parsing/ConfigFile.h>
#include <Springbok/Graphics/BitmapFont.h>
#include <Springbok/Audio/SoundSource.h>

struct AssetDefinition
{
	void initAll();
	
	// BULLET DEFINITIONS
	
	Bullet::_Definition PhaserBullet;
	Bullet::_Definition MiniGunBullet;
	Bullet::_Definition GunBullet;
	MissileDefinition HomingMissile;
	
	// WEAPON DEFINITIONS
	
	Ship::_Weapon Phaser;  // Weak and slow
	Ship::_Weapon MiniGun; // Weak and fast
	Ship::_Weapon BigGun;  // Strong and slow
	Ship::_Weapon MissileLauncher; // Very slow, strong, homing
	
	// PARTICLE DEFINITIONS
	
	Particle::_Definition EnergyShield;
	Particle::_Definition EnergyShieldDestruction;
	
	Particle::_Definition PulseEngineGlow;
	Particle::_Definition PulseEngineSpark;
	
	Particle::_Definition GlowParticle;
	Particle::_Definition SparkParticle;
	
	Particle::_Definition ExplosionCloud;
	Particle::_Definition ExplosionCloudAdditive;
	Particle::_Definition ExplosionSparks;
	Particle::_Definition ExplosionFlash;
	Particle::_Definition ExplosionShockwave;
	
	// IMAGE DEFINITIONS
	
	Image ShieldRechargeSprite;
	Image ShieldStaticSprite;
	Image GlowSprite;
	Image ParticleAtlas;
	
	struct
	{
		Image Fog1;
		Image Fog2;
		Image Stars;
	} BG;
	
	struct
	{
		Image BgCloud;
		Image Glow1;
		Image Glow2;
		Image Sprite;
	} Sun;
	
	Image DottedLine;
	
	// SHIP DEFINITIONS
	
	Ship Fighter;
	Ship Warship;
	
	// FONT DEFINITIONS
	
	BitmapFont Font;
	
	// MUSIC DEFINITIONS
	
	SoundSource MusicMainTheme;
	
	// SOUND DEFINITIONS
	
	SoundSource SoundSimpleShot;
	SoundSource SoundHeavyShot;
	SoundSource SoundBoost;
	SoundSource SoundExplosion01;
	SoundSource SoundExplosion02;
	
	// SHADER DEFINITIONS
	
	ShaderProgram ToneMapping;
	
	// CONFIG FILE
	
	ConfigFile Config;
	
private:
	void initBullets();
	void initWeapons();
	void initParticles();
	void initImages();
	void initFonts();
	void initSounds();
	void initShips();
	void initShaders();
	void initMisc();
};

extern AssetDefinition gAssets;