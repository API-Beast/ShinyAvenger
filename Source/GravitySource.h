//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once
#include "PhysicsObject.h"

#include <Springbok/Graphics/Image.h>
#include <Springbok/Graphics/Color.h>

class PlaySpace;

struct GravitySource
{
public:
	GravitySource(Vec2F pos, float grav, float range, Color color, Color hcolor)
	 : Position(pos),Gravity(grav),Range(range),CenterColor(color),HighlightColor(hcolor),BackgroundColor(CenterColor*0.3f){};
	GravitySource() = default;
	void update(float dt, PlaySpace* space);
	void draw(BatchRenderer2D& r);
	void drawTop(BatchRenderer2D& r);
	void influence(PhysicsObject* obj, float dt);
public:
	Vec2F Position;
	float Gravity = 1;
	float Range = 500;
	
	Color CenterColor;
	Color HighlightColor;
	Color BackgroundColor;
	
	float TimeSinceSpawn = 0.0f;
	float CenterSize = 1.0f;
	
	BoundingRect Bounds;
	
	Image CloudSprite = Image("GravityCloud.png");
	Image Sprite = Image("GravitySource.png");
	Image HighlightSprite = Image("GravitySourceHighlight.png");
	Image HighlightSprite2 = Image("GravitySourceHighHighlight.png");
};