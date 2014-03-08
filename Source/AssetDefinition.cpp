//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "AssetDefinition.h"

void AssetDefinition::initAll()
{
	initParticles();
	initBullets();
	initWeapons();
	initImages();
	initFonts();
	initSounds();
}

void AssetDefinition::initImages()
{
	ShieldRechargeSprite = Image("Ship/ShieldRecharge.png");
	ShieldStaticSprite = Image("Ship/ShieldStatic.png");
	GlowSprite = Image("Glow.png");
}

void AssetDefinition::initFonts()
{	
	BigFont.loadRange(Image("UI/BigNumbers.png"), '\'', ':');
	SmallFont.loadGrid(Image("UI/SmallFont.png"), 0, 16);	
}

void AssetDefinition::initSounds()
{
	MusicMainTheme = new SoundSource("Sounds/MainTheme.ogg");
}


void AssetDefinition::initBullets()
{
	// ------------------------------------------------------------------
	// ### Phaser
	// ------------------------------------------------------------------
	{
		Bullet::_Definition& b = PhaserBullet;
		
		b.PhysicsProperties.Mass = 5;
		b.PhysicsProperties.Drag = 0;
		b.PhysicsProperties.Flow = 5;
		b.Lifetime = 1.f;
		b.Power = 10.f;
		b.Speed = 800.f;

		b.Blending = RenderContext::Additive;
		
		b.ColorAnimation.insert(0.5, Palette::Orange);
		b.ColorAnimation.insert(1.0, Palette::Red);

		b.AlphaAnimation.insert(0.8, 1.f);
		b.AlphaAnimation.insert(1.0, 0.f);

		b.ScaleAnimation.insert(0.2, {1.f, 4.f});
		b.ScaleAnimation.insert(0.8, {1.5f, 3.f});
		b.ScaleAnimation.insert(1.0, {0.5f, 4.f});

		b.Sprite = Image("Player/Bullet.png");
	}
	
	// ------------------------------------------------------------------
	// ### Minigun
	// ------------------------------------------------------------------
	{
		Bullet::_Definition& b = MiniGunBullet;
		
		b.Bullet::_Definition::operator=(PhaserBullet);
		b.Blending = RenderContext::Additive;
		b.PhysicsProperties.Flow = 0;
		b.PhysicsProperties.Drag = 0;
		b.PhysicsProperties.Mass = 0.2;
		b.Power = 5.f;
		b.Speed = 1600.f;
		
		b.ColorAnimation = Palette::Cyan;
		b.ColorAnimation.insert(1.0, Palette::Blue*0.2);
	}
	
	// ------------------------------------------------------------------
	// ### Gun
	// ------------------------------------------------------------------
	{
		Bullet::_Definition& b = GunBullet;
		/*TODO*/
		b.Power = 20;
	}
	
	// ------------------------------------------------------------------
	// ### Missile
	// ------------------------------------------------------------------
	{
		MissileDefinition& b = HomingMissile;
		
		b.Sprite = Image("Ship/Missile.png");
		b.Power = 60;
		b.Speed = 1000;
		b.Explodes = true;
		
		b.RotationRate = 20.f;
		b.Acceleration = 2000;
	}
}

void AssetDefinition::initWeapons()
{
	// ------------------------------------------------------------------
	// ### Phaser
	// ------------------------------------------------------------------
	{
		Ship::_Weapon& w = Phaser;
		
		w.BulletPrototype = PhaserBullet;
		w.Bullets = 2;
		w.ShotDelay = 0.50f;
		w.Spread = 0.02_turn;
		w.Type = Ship::MultiShot;
	}
	
	// ------------------------------------------------------------------
	// ### Minigun
	// ------------------------------------------------------------------
	{
		Ship::_Weapon& w = MiniGun;
		
		w.BulletPrototype = MiniGunBullet;
		w.Bullets = 3;
		w.ShotDelay = 0.10f;
		w.Spread = 0.05_turn;
	}
	
	// ------------------------------------------------------------------
	// ### Gun 
	// ------------------------------------------------------------------
	{
		Ship::_Weapon& w = BigGun;
		
		w.BulletPrototype = GunBullet;
		w.Bullets = 2;
		w.ShotDelay = 0.40f;
		w.Spread = 0.0_turn;
	}
	
	// ------------------------------------------------------------------
	// ### Missile Launch
	// ------------------------------------------------------------------
	{
		Ship::_Weapon& w = MissileLauncher;
		
		w.BulletPrototype = HomingMissile;
		w.Bullets = 1;
		w.ShotDelay = 1.00f;
		w.Spread = 0.125_turn;
	}
}


void AssetDefinition::initParticles()
{
	//  ----------------------------------------------
	//  ### Pulse Engine Glow           
	//  ----------------------------------------------
	{
		Particle::_Definition& p = PulseEngineGlow;
		
		p.Sprite = Image("Player/Impulse.png");
		p.DrawMode = RenderContext::Additive;
		p.Lifetime = 0.3f;
		p.PhysicsProperties.Drag = 2;
		p.PhysicsProperties.Mass = 0.5f;
		
		{
			auto& anim = p.Animation.Alpha;
			
			anim.insert(0.0f, 0.0f);
			anim.insert(0.2f, 0.3f);
			anim.insert(1.0f, 0.0f);
		}
		
		{
			auto& anim = p.Animation.Col;
			
			anim.clear();
			anim.insert(0.0f, Colors::White);
			anim.insert(1.0f, Palette::Red);
		}
		
		{
			auto& anim = p.Animation.Scale;
			
			anim.clear();
			anim.insert(0.0f, Vec2F(1.5f, 1.5f));
			anim.insert(0.2f, Vec2F(1.0f, 1.0f));
			anim.insert(0.3f, Vec2F(1.0f, 2.0f));
			anim.insert(1.0f, Vec2F(1.0f, 3.0f));
		}
	}
	
	//  ------------------------------------------------------------------------------
	//  ### Pulse Engine Spark           
	//  ------------------------------------------------------------------------------
	{
		Particle::_Definition& p = PulseEngineSpark;
		
		p = PulseEngineGlow;
		p.Sprite = Image("Player/Spark.png");
		p.PhysicsProperties.Drag = 1;
		p.PhysicsProperties.Mass = 1;
		p.Lifetime = 0.15f;
		
		{
			auto& anim = p.Animation.Alpha;
			
			anim.clear();
			anim.insert(0.0f, 0.0f);
			anim.insert(0.2f, 1.0f);
			anim.insert(1.0f, 0.0f);
		}
	}
	
	// ------------------------------------------------------------------------------
	// ### Energy Shield
	// ------------------------------------------------------------------------------
	{
		Particle::_Definition& p = EnergyShield;
		
		p.Sprite = Image("Ship/ShieldImpact.png");
		p.DrawMode = RenderContext::Additive;
		p.Lifetime = 0.2f;
		p.PhysicsProperties.Drag = 0.f;
		
		{
			auto& anim = p.Animation.Alpha;
			
			anim.clear();
			anim.insert(0.0f, 1.0f);
			anim.insert(0.5f, 0.2f);
			anim.insert(1.0f, 0.0f);
		}
		
		p.Animation.Col.insert(1.0f, Color(0.0f, 0.0f, 0.4f));
	}
	
	// ------------------------------------------------------------------
	// ### Energy Shield Destruction
	// ------------------------------------------------------------------
	{
		Particle::_Definition& p = EnergyShieldDestruction;
		
		p = EnergyShield;
		p.Sprite = Image("Ship/ShieldDestruction.png");
	}
	
	// ------------------------------------------------------------------
	// ### Glow Particle
	// ------------------------------------------------------------------
	{
		Particle::_Definition& p = GlowParticle;
		
		p.Sprite = Image("Glow.png");
		p.DrawMode = RenderContext::Additive;
		p.Lifetime = 0.2f;
		p.PhysicsProperties.Mass = 0.f;
		p.PhysicsProperties.Drag = 0.0f;
		p.PhysicsProperties.Stabilizer = 0.0f;
		
		{
			auto& anim = p.Animation.Alpha;
			
			anim.clear();
			anim.insert(0.0f, 0.4f);
			anim.insert(0.5f, 0.1f);
			anim.insert(1.0f, 0.0f);
		}
	}
	
	// ------------------------------------------------------------------
	// ### Spark Particle
	// ------------------------------------------------------------------
	{
		Particle::_Definition& p = SparkParticle;
		
		p.Sprite = Image("spark.png");
		p.DrawMode = RenderContext::Additive;
		p.Lifetime = 0.2f;
		p.PhysicsProperties.Mass = 2.f;
		p.PhysicsProperties.Drag = 1.f;
		
		{
			auto& anim = p.Animation.Alpha;
			
			anim.clear();
			anim.insert(0.0f, 1.0f);
			anim.insert(0.7f, 1.0f);
			anim.insert(1.0f, 0.0f);
		}
		
		{
			auto& anim = p.Animation.Col;
			
			anim.clear();
			anim.insert(0.00f, RGB(1.0f, 1.0f, 1.0f));
			anim.insert(0.70f, RGB(1.0f, 0.9f, 0.8f));
			anim.insert(0.95f, RGB(1.0f, 0.0f, 0.0f));
		}
		
		{
			auto& anim = p.Animation.Scale;
			
			anim.clear();
			anim.insert(0.0f, Vec2F(1.f, 0.5f));
			anim.insert(0.7f, Vec2F(1.f, 1.f));
			anim.insert(1.0f, Vec2F(1.f, 2.f));
		}
	}
}


AssetDefinition gAssets;