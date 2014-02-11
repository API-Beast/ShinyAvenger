//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Weapon.h"
#include <Springbok/Graphics/Image.h>

Weapon::Weapon()
{
	BulletPrototype.Mass = 5;
	BulletPrototype.Drag = 5;
	BulletPrototype.Lifetime = 1.f;
	
	BulletPrototype.ColorAnimation[0.0].insert(Colors::White);
	BulletPrototype.ColorAnimation[0.5].insert(Colors::Saturated::Orange);
	BulletPrototype.ColorAnimation[1.0].insert(Colors::Saturated::Red);
	
	BulletPrototype.GlowColorAnimation[0.0].insert(Colors::Saturated::Yellow);
	BulletPrototype.GlowColorAnimation[0.5].insert(Colors::Saturated::Orange);
	BulletPrototype.GlowColorAnimation[1.0].insert(Colors::Saturated::Red);
	
	BulletPrototype.AlphaAnimation[0.0].insert(1.f);
	BulletPrototype.AlphaAnimation[0.8].insert(1.f);
	BulletPrototype.AlphaAnimation[1.0].insert(0.f);
	
	BulletPrototype.ScaleAnimation[0.0].insert(1.f);
	BulletPrototype.ScaleAnimation[0.2].insert(Vec2F(1.f, 4.f));
	BulletPrototype.ScaleAnimation[0.8].insert(Vec2F(1.5f, 3.f));
	BulletPrototype.ScaleAnimation[1.0].insert(Vec2F(0.5f, 4.f));
	
	BulletPrototype.GlowScaleAnimation[0.0].insert(2.f);
	BulletPrototype.GlowScaleAnimation[1.0].insert(2.f);
	
	BulletPrototype.Sprite = Image("Player/Bullet.png");
	BulletPrototype.Glow = Image("Glow.png");
}

void Weapon::update(float Time, PlaySpace* Space, Ship* TheShip)
{
	if(isShooting)
	{
		ShootTimer -= t;
		if(ShootTimer < 0)
		{
			ShootTimer = 0.2f;
			Bullet bullet(BulletPrototype);
			bullet.Rotation = TheShip->Rotation;
			bullet.Speed = TheShip->Rotation.toDirection()*1000 + TheShip->Speed *0.8;
			bullet.Position = TheShip->Position + (TheShip->Rotation+0.25f).toDirection()*10;
			Space->spawnPlayerBullet(bullet);
			bullet.Position = TheShip->Position - (TheShip->Rotation+0.25f).toDirection()*10;
			Space->spawnPlayerBullet(bullet);
		}
	}
}

