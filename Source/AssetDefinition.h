//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Bullet.h"
#include "Particle.h"
#include "Ship.h"

#include <Springbok/Graphics/BitmapFont.h>

struct AssetDefinition
{
	void initAll();
	
	Bullet::_Definition PhaserBullet;
	Bullet::_Definition MiniGunBullet;
	Bullet::_Definition GunBullet;
	MissileDefinition HomingMissile;
	
	Ship::_Weapon Phaser;  // Weak and slow
	Ship::_Weapon MiniGun; // Weak and fast
	Ship::_Weapon BigGun;  // Strong and slow
	Ship::_Weapon MissileLauncher; // Very slow, strong, homing
	
	Particle::_Definition EnergyShield;
	Particle::_Definition EnergyShieldDestruction;
	
	Particle::_Definition PulseEngineGlow;
	Particle::_Definition PulseEngineSpark;
	
	Particle::_Definition GlowParticle;
	Particle::_Definition SparkParticle;
	
	Particle::_Definition ExplosionCloud;
	Particle::_Definition ExplosionSparks;
	
	Image ShieldRechargeSprite;
	Image ShieldStaticSprite;
	Image GlowSprite;
	
	BitmapFont BigFont;
	BitmapFont SmallFont;
private:
	void initBullets();
	void initWeapons();
	void initParticles();
	void initImages();
	void initFonts();
};

extern AssetDefinition gAssets;