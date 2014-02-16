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
	r.Rotation = TimeSinceSpawn * 0.040_turn;
	r.Scale = 1.2f;
	r.setColor(BackgroundColor*0.2f);
	GlowSprite.drawStretched(Range*2, r);
	
	// Background 2
	r.Rotation = TimeSinceSpawn * 0.070_turn;
	r.Scale = 1.0f;
	r.setColor(BackgroundColor, 0.9f);
	r.setBlendingMode(RenderContext::Additive);
	GlowSprite.drawStretched(Range*2, r);
	
	// Background 3
	r.Rotation = TimeSinceSpawn * 0.090_turn;
	r.Scale = 0.3f;
	r.setColor(Colors::Black, 0.5f);
	r.setBlendingMode(RenderContext::Default);
	GlowSprite.drawStretched(Range*2, r);
	
	// Background Center 1
	r.Rotation = TimeSinceSpawn * 0.20_turn;
	r.Scale = CenterSize * 1.5f;
	r.setColor(CenterColor, 0.9f);
	Graphics.draw(r);
	
	// Background Center 2
	r.Rotation = TimeSinceSpawn * 0.25_turn;
	r.Scale = CenterSize * 1.1f;
	r.setColor(CenterColor);
	r.setBlendingMode(RenderContext::Default);
	Graphics.draw(r);
	
	// Background Center Highlight
	r.Rotation = TimeSinceSpawn * 0.30_turn;
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

