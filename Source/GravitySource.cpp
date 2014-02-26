//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "GravitySource.h"
#include "AssetDefinition.h"
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
}

void GravitySource::drawTop(RenderContext r)
{
	r.Offset = Position;
	
	r.Rotation = 0_turn;
	r.Scale = 1.0f;
	r.setColor(CenterColor, 0.2f);
	r.setBlendingMode(RenderContext::Additive);
	gAssets.GlowSprite.drawStretched(Range, r);
	r.setColor(HighlightColor, 0.4f);
	gAssets.GlowSprite.drawStretched(Range/3, r);
	
	float phase[] = {0.f, 0.f, 0.f};
	phase[0] = (TimeSinceSpawn * 0.25_turn).sin() + 0.7f;
	phase[1] = 0.5-phase[0] + (TimeSinceSpawn * 0.30_turn).cos()*0.3f;
	
	phase[0] *= phase[0];
	phase[1] *= phase[1];
	phase[2]  = 1-phase[0]-phase[1] + (TimeSinceSpawn * 0.45_turn + 0.20_turn).cos()*0.3f;
	
	phase[0] = BoundBy<float>(0, phase[0], 1.2f);
	phase[1] = BoundBy<float>(0, phase[1], 1.2f);
	phase[2] = BoundBy<float>(0, phase[2], 0.8f);
	
	for (int i = 0; i < 3; ++i)
	{
		r.Rotation = TimeSinceSpawn * (0.08_turn + 0.02_turn * i) + (0.99_turn / 3 * i);
		r.Scale = CenterSize;
		r.setColor(CenterColor, Min(phase[i], 1.0f));
		r.setBlendingMode(RenderContext::Default);
		Graphics.draw(r);
	}
	
	for (int i = 0; i < 3; ++i)
	{
		r.Rotation = TimeSinceSpawn * (0.08_turn + 0.02_turn * i) + (0.99_turn / 3 * i);
		r.Scale = CenterSize;
		r.setColor(HighlightColor, Min(phase[i]/3, 1.0f));
		r.setBlendingMode(RenderContext::Additive);
		HighlightGraphics.draw(r);
	}
	
	for (int i = 0; i < 3; ++i)
	{
		r.Rotation = TimeSinceSpawn * (0.08_turn + 0.02_turn * i) + (1.3_turn / 3 * i);
		r.Scale = CenterSize;
		r.setColor(Colors::White, Min(phase[i]/2, 1.0f));
		r.setBlendingMode(RenderContext::Additive);
		HighlightGraphics2.draw(r);
	}
}

void GravitySource::influence(PhysicsObject* obj, float dt)
{
	Vec2F difference = (Position - obj->Position);
	float distance = difference.getLength();
	Vec2F direction = difference.normalized();
	obj->Speed += ((direction * Gravity * obj->Mass) / Max((distance/Range)*(distance/Range), 1.0f))*dt;
}

