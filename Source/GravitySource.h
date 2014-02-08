//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once
#include "PhysicsObject.h"

#include <Springbok/Graphics/Image.h>

struct GravitySource
{
public:
	GravitySource(Vec2F pos, float grav, float range, Color color, Color hcolor)
	 : Position(pos),Gravity(grav),Range(range),CenterColor(color),HighlightColor(hcolor){};
	GravitySource() = default;
	void update(float dt);
	void draw(RenderContext r);
	void influence(PhysicsObject* obj, float dt);
public:
	Vec2F Position;
	float Gravity;
	float Range;
	
	Color CenterColor;
	Color HighlightColor;
	
	float TimeSinceSpawn = 0.0f;
	
	Image GlowSprite = Image("Glow.png");
	Image Graphics = Image("GravitySource.png");
	Image HighlightGraphics = Image("GravitySourceHighlight.png");
};