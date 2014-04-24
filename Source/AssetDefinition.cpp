//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "AssetDefinition.h"

void AssetDefinition::initAll()
{
	initImages();
	initFonts();
	initSounds();
	initShaders();
	initMisc();
	
	// Gameplay
	initParticles();
	initBullets();
	initWeapons(); // Requires bullets to be initialized first
	initShips();   // Requires weapons to be initialized first
}

void AssetDefinition::initImages()
{
	ShieldRechargeSprite = Image("Ship/ShieldRecharge.png");
	ShieldStaticSprite = Image("Ship/ShieldStatic.png");
	ParticleAtlas = Image("Particles/Atlas.png");
	DottedLine = Image("HUD/DottedLine.png");
	GlowSprite = ParticleAtlas.cut({0, 128}, {256, 256});
	
	BG.Fog1  = Image("Background/Fog01.png");
	BG.Fog2  = Image("Background/Fog02.png");
	BG.Stars = Image("Background/Stars.png");
	
	Sun.BgCloud = Image("Sun/Background.png");
	Sun.Glow1   = Image("Sun/Glow01.png");
	Sun.Glow2   = Image("Sun/Glow02.png");
	Sun.Sprite  = Image("Sun/Sprite.png");
}

void AssetDefinition::initShips()
{
	Fighter.Sprite           = Image("Ship/Fighter.png");
	Fighter.Overlay          = Image("Ship/FighterOverlay.png");
	Fighter.PrimaryWeapon    = Phaser;
	Fighter.SecondaryWeapon  = MissileLauncher;
	Fighter.Pictogram        = Image("Ship/FighterPicture.png");
	Fighter.PictogramOverlay = Image("Ship/FighterPictureOverlay.png");
	Fighter.ImpulseColor     = Palette::Lilac;
	
	Warship.Sprite           = Image("Ship/Player.png");
	Warship.Overlay          = Image("Ship/PlayerOverlay.png");
	Warship.PrimaryWeapon    = MiniGun;
	Warship.SecondaryWeapon  = MissileLauncher;
	Warship.Pictogram        = Image("Ship/FighterPicture.png");
	Warship.PictogramOverlay = Image("Ship/FighterPictureOverlay.png");
	Warship.ImpulseColor     = RGB(0.4f, 0.8f, 1.0f);
}

void AssetDefinition::initFonts()
{
	Font.loadGrid(Image("HUD/Font16.png"), 0, 16);
}

void AssetDefinition::initSounds()
{
	MusicMainTheme   = SoundSource("Music/MainTheme.ogg");
	
	SoundSimpleShot  = SoundSource("Sounds/Bullets/Energy01.ogg");
	SoundHeavyShot   = SoundSource("Sounds/Bullets/Energy02.ogg");
	
	SoundBoost       = SoundSource("Sounds/Boost.ogg");
	
	SoundExplosion01 = SoundSource("Sounds/Explosion01.ogg");
	SoundExplosion02 = SoundSource("Sounds/Explosion02.ogg");
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

		b.BlendMode = Blending::Additive;
		
		b.ColorAnimation.insert(0.5, Palette::Orange);
		b.ColorAnimation.insert(1.0, Palette::Red);

		b.AlphaAnimation.insert(0.8, 1.f);
		b.AlphaAnimation.insert(1.0, 0.f);

		b.ScaleAnimation.insert(0.2, {1.f, 4.f});
		b.ScaleAnimation.insert(0.8, {1.5f, 3.f});
		b.ScaleAnimation.insert(1.0, {0.5f, 4.f});

		b.Sprite = Image("Weapons/EnergyBullet.png");
	}
	
	// ------------------------------------------------------------------
	// ### Minigun
	// ------------------------------------------------------------------
	{
		Bullet::_Definition& b = MiniGunBullet;
		
		b.Bullet::_Definition::operator=(PhaserBullet);
		b.BlendMode = Blending::Additive;
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
		
		b.Sprite = Image("Weapons/Missile.png");
		b.Power = 60;
		b.Speed = 400;
		b.Explodes = true;
		b.Glow = false;
		b.Lifetime = 10.f;
		
		b.RotationRate = 40.f;
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
		
		w.ReloadDelay = 0.15f;
		w.MaxReloadStack = 20;
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
		w.Bullets = 2;
		
		w.ShotDelay = 0.15f;
		w.MaxReloadStack = 3;
		w.ReloadDelay = 2.00f;
		w.Spread = 0.0_turn;
	}
}


void AssetDefinition::initParticles()
{
	ParticleAtlas = Image("Particles/Atlas.png");
	
	//  ----------------------------------------------
	//  ### Pulse Engine Glow           
	//  ----------------------------------------------
	{
		Particle::_Definition& p = PulseEngineGlow;
		
		p.Sprite = ParticleAtlas.cut({256, 128}, {64, 64});
		p.DrawMode = Blending::Additive;
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
		p.Sprite = ParticleAtlas.cut({320, 128}, {32, 32});
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
		
		p.Sprite = ParticleAtlas.cut({256, 192}, {128, 128});
		p.DrawMode = Blending::Additive;
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
		p.Sprite = ParticleAtlas.cut({256, 320}, {128, 128});
	}
	
	// ------------------------------------------------------------------
	// ### Glow Particle
	// ------------------------------------------------------------------
	{
		Particle::_Definition& p = GlowParticle;
		
		p.Sprite = GlowSprite;
		p.DrawMode = Blending::Additive;
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
		
		p.Sprite = ParticleAtlas.cut({384, 0}, {8, 75});
		p.DrawMode = Blending::Additive;
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
	
	// ------------------------------------------------------------------
	// ### Explosion Cloud
	// ------------------------------------------------------------------
	{
		Particle::_Definition& p = ExplosionCloud;
		
		p.Sprite = ParticleAtlas.cut({0, 0}, {128, 128});
		p.DrawMode = Blending::Alpha;
		p.Lifetime = 0.5f;
		p.PhysicsProperties.Mass = -1.f;
		p.PhysicsProperties.Drag = 3.f;
		
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
			anim.insert(0.70f, RGB(1.0f, 0.6f, 0.4f));
			anim.insert(0.95f, RGB(0.3f, 0.3f, 0.3f));
		}
		
		{
			auto& anim = p.Animation.Scale;
			
			anim.clear();
			anim.insert(0.0f, {0.f, 1.f});
			anim.insert(0.02f, {1.f, 1.f});
			anim.insert(0.6f, {2.f, 2.f});
			anim.insert(1.0f, {3.f, 4.f});
		}
		
		{
			auto& anim = p.Animation.Rotation;
			
			anim.clear();
			anim.insert(0.0f, 0.0_turn);
			anim.insert(0.8f, 0.2_turn);
			anim.insert(1.0f, 0.8_turn);
		}
	}
	
	ExplosionCloudAdditive = ExplosionCloud;
	ExplosionCloudAdditive.DrawMode = Blending::Additive;
	
	// ------------------------------------------------------------------
	// ### Explosion Flash
	// ------------------------------------------------------------------
	{
		Particle::_Definition& p = ExplosionFlash;
		
		p = GlowParticle;
		p.Sprite = ParticleAtlas.cut({0, 0}, {128, 128});

		{
			auto& anim = p.Animation.Scale;
			
			anim.clear();
			anim.insert(0.0f, 0.f);
			anim.insert(0.1f, 2.f);
			anim.insert(0.3f, 1.f);
		}
	}
	
	ExplosionSparks = SparkParticle;
	
	// ------------------------------------------------------------------
	// ### Explosion Shockwave
	// ------------------------------------------------------------------
	{
		Particle::_Definition& p = ExplosionShockwave;
		p = GlowParticle;
		p.Sprite = ParticleAtlas.cut({128, 0}, {128, 128});
		p.Lifetime = 0.6f;
		
		{
			auto& anim = p.Animation.Scale;
			
			anim.clear();
			anim.insert(0.0f, {0.f, 1.f});
			anim.insert(0.1f, {1.f, 1.f});
			anim.insert(1.0f, {3.f, 1.f});
		}
		
		{
			auto& anim = p.Animation.Alpha;
			
			anim.clear();
			anim.insert(0.0f, 0.0f);
			anim.insert(0.2f, 1.0f);
			anim.insert(0.7f, 0.2f);
			anim.insert(1.0f, 0.0f);
		}
	}
}

void AssetDefinition::initShaders()
{
	ToneMapping = ShaderProgram("Shaders/ToneMap.Fragment.glsl");
	ToneMapping.FragmentShader->compile();
	//ToneMapping.bindVertexAttribute(0, "Position");
	//ToneMapping.bindVertexAttribute(1, "TextureCoordinate");
	//ToneMapping.bindVertexAttribute(2, "Color");
	ToneMapping.link();
}

void AssetDefinition::initMisc()
{
	Config.loadFromFile("Config.xini");
}

AssetDefinition gAssets;