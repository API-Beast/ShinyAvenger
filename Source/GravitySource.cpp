//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "GravitySource.h"
#include <GL/gl.h>

void GravitySource::update(float dt, PlaySpace* space)
{
	TimeSinceSpawn += dt;
}

void GravitySource::draw(RenderContext r)
{
	r.Offset = Position;
	
	// Background 1
	r.Rotation = TimeSinceSpawn * 0.15_turn;
	r.Scale = 1.2f;
	r.setColor(Colors::Black, 0.4f);
	GlowSprite.drawStretched(Range*2, r);
	
	// Background 2
	r.Rotation = TimeSinceSpawn * 0.32_turn;
	r.Scale = 1.0f;
	r.setColor(Colors::Black, 0.6f);
	GlowSprite.drawStretched(Range*2, r);
	
	// Background Center 1
	r.Rotation = TimeSinceSpawn * 0.21_turn;
	r.Scale = CenterSize * 1.5f;
	r.setColor(CenterColor, 0.7f);
	Graphics.draw(r);
	
	// Background Center 2
	r.Rotation = TimeSinceSpawn * 0.36_turn;
	r.Scale = CenterSize * 1.1f;
	r.setColor(CenterColor);
	Graphics.draw(r);
	
	// Background Center Highlight
	r.Rotation = TimeSinceSpawn * 0.40_turn;
	r.Scale = CenterSize;
	r.setColor(HighlightColor);
	r.setBlendingMode(RenderContext::Additive);
	HighlightGraphics.draw(r);
}

void GravitySource::influence(PhysicsObject* obj, float dt)
{
	Vec2F difference = (Position - obj->Position);
	float distance = difference.getLength();
	Vec2F direction = difference.normalized();
	obj->Speed += ((direction * Gravity * obj->Mass) / Max((distance/Range)*(distance/Range), 1.0f))*dt;
}

