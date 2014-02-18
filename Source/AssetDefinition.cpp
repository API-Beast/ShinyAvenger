//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "AssetDefinition.h"

void AssetDefinition::initAll()
{
	PulseEngineGlow.Sprite = Image("Player/Impulse.png");
	PulseEngineGlow.DrawMode = RenderContext::Additive;
	PulseEngineGlow.Lifetime = 0.2f;
	PulseEngineGlow.PhysicsProperties.Drag = 10;
	PulseEngineGlow.PhysicsProperties.Mass = 0.5f;
	
	{
		auto& anim = PulseEngineGlow.Animation.Alpha;
		anim.insert(1.0f, 0.0f);
	}
	
	{
		auto& anim = PulseEngineGlow.Animation.Color;
		anim.clear();
		anim.insert(0.0f, Colors::Saturated::Orange);
		anim.insert(1.0f, Colors::Saturated::Magenta);
	}
	
	{
		auto& anim = PulseEngineGlow.Animation.Scale;
		anim.clear();
		anim.insert(0.0f, Vec2F(1.0f, 1.0f));
		anim.insert(0.3f, Vec2F(1.0f, 2.0f));
		anim.insert(1.0f, Vec2F(1.0f, 3.0f));
	}
	
	PulseEngineSpark = PulseEngineGlow;
	PulseEngineSpark.Sprite = Image("Player/Spark.png");
	PulseEngineSpark.PhysicsProperties.Drag = 0;
	PulseEngineSpark.PhysicsProperties.Mass = 5;
	PulseEngineSpark.Lifetime = 0.15f;
	
	{
		auto& anim = PulseEngineSpark.Animation.Alpha;
		anim.clear();
		anim.insert(0.0f, 0.0f);
		anim.insert(0.2f, 1.0f);
		anim.insert(1.0f, 0.0f);
	}
	
	EnergyShield.Sprite = Image("Ship/Shield.png");
	EnergyShield.DrawMode = RenderContext::Additive;
	EnergyShield.Lifetime = 0.2f;
	EnergyShield.PhysicsProperties.Drag = 0.f;
	
	{
		auto& anim = EnergyShield.Animation.Alpha;
		anim.clear();
		anim.insert(0.0f, 1.0f);
		anim.insert(0.5f, 0.2f);
		anim.insert(1.0f, 0.0f);
	}
	
	EnergyShield.Animation.Color.insert(1.0f, Color(0.0f, 0.0f, 0.4f));
	EnergyShield.Animation.Scale = Vec2F(0.75f, 0.75f);
}

AssetDefinition gAssets;