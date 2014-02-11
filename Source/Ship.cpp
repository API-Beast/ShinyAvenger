//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Ship.h"
#include "Behavior.h"

Ship::Ship(ColorRGB FractionColor, Movement *TheMovement, Behavior *TheBehavior)
: FractionColor(FractionColor), TheBehavior(TheBehavior), TheMovement(TheMovement)
{
	ImpulseParticle.Sprite = Image("Player/Impulse.png");
	ImpulseParticle.DrawMode = RenderContext::Additive;
	ImpulseParticle.Lifetime = 0.2f;
	ImpulseParticle.Drag = 10;
	ImpulseParticle.Mass = 0.5f;
	
	ImpulseParticle.Animation.Alpha[0.0].insert(1.0f);
	ImpulseParticle.Animation.Alpha[1.0].insert(0.0f);
	
	ImpulseParticle.Animation.Color[0.0].insert(Colors::Saturated::Orange);
	ImpulseParticle.Animation.Color[1.0].insert(Colors::Saturated::Magenta);
	
	ImpulseParticle.Animation.Scale[0.0].insert(Vec2F(1.0f, 1.0f));
	ImpulseParticle.Animation.Scale[0.3].insert(Vec2F(1.0f, 2.0f));
	ImpulseParticle.Animation.Scale[1.0].insert(Vec2F(1.0f, 3.0f));
	
	ImpulseParticle.Animation.Rotation[0.0].insert(0.0f);
	ImpulseParticle.Animation.Rotation[1.0].insert(0.0f);
	
	SparkParticle = ImpulseParticle;
	SparkParticle.Sprite = Image("Player/Spark.png");
	SparkParticle.Drag = 0;
	SparkParticle.Mass = 5;
	SparkParticle.Lifetime = 0.15f;
	
	SparkParticle.Animation.Alpha = KeyframeList<float>();
	SparkParticle.Animation.Alpha[0.0].insert(0.0f);
	SparkParticle.Animation.Alpha[0.2].insert(1.0f);
	SparkParticle.Animation.Alpha[1.0].insert(0.0f);
}

void Ship::update(float t, PlaySpace* space)
{	
	if (TheBehavior != NULL)
	{
		TheBehavior->update(t, this);
	}	
	
	TheMovement->update(t, this, space);	
	TheWeapon.update(t, space, this);	
}

void Ship::draw(RenderContext r)
{
	r.setColor(FractionColor, 1.f);
	r.Offset = Position;
	r.Rotation = Rotation;
	Sprite.draw(r);
}