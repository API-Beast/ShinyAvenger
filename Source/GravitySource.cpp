//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "GravitySource.h"
#include "AssetDefinition.h"
#include "PlaySpace.h"
#include <Springbok/Graphics/Transform2D.h>
#include <Springbok/Graphics/BatchRenderer.h>
#include <GL/gl.h>

void GravitySource::update(float dt, PlaySpace* space)
{
	TimeSinceSpawn += dt;
	auto ships = space->findShips(Position - CenterSize*950, Position + CenterSize*950);
	for(Ship* ship : ships)
	{
		if(InRange(ship->Position, Position, CenterSize * 450 + 600))
			ship->doDamage( 5*dt, space);
		if(InRange(ship->Position, Position, CenterSize * 450 + 300))
			ship->doDamage(10*dt, space);
		if(InRange(ship->Position, Position, CenterSize * 450 + 100))
			ship->doDamage(25*dt, space);
		if(InRange(ship->Position, Position, CenterSize * 450 + 50))
			ship->doDamage(100*dt, space);
		if(InRange(ship->Position, Position, CenterSize * 450))
			ship->doDamage(400*dt, space);
	}
}

void GravitySource::draw(BatchRenderer2D& r)
{
	Transform2D t = Position2D(Position);
	
	// Three different background layers
	r.draw(CloudSprite, t + Scale2D(Range * 2.4f / 1024) + Rotate2D(TimeSinceSpawn * 0.040_turn), Vec4F{BackgroundColor*0.2f});
	r.draw(CloudSprite, t + Scale2D(Range * 2.0f / 1024) + Rotate2D(TimeSinceSpawn * 0.070_turn), Vec4F{BackgroundColor*0.2f, 0.9f});
	r.draw(CloudSprite, t + Scale2D((256 + Range * 0.25f) / 1024) + Rotate2D(TimeSinceSpawn * 0.090_turn), Vec4F{Colors::Black, 0.5f});
}

void GravitySource::drawTop(BatchRenderer2D& r)
{	
	// Calculate phases.
	float phase[] = {0.5f, 0.f, 0.f, 0.f};
	phase[1] = (TimeSinceSpawn * 0.1_turn).sin() + 0.7f;
	phase[2] = 0.5-phase[1] + (TimeSinceSpawn * 0.3_turn).cos()*0.2f;
	
	phase[1] *= phase[1];
	phase[2] *= phase[2];
	phase[3]  = 1-phase[1]-phase[2] + (TimeSinceSpawn * 0.4_turn + 0.20_turn).cos()*0.2f;
	
	phase[1] = BoundBy<float>(0, phase[1], 1.2f);
	phase[2] = BoundBy<float>(0, phase[2], 1.2f);
	phase[3] = BoundBy<float>(0, phase[3], 0.8f);
	
	// Draw.
	Transform2D t = Position2D(Position);
	
	r.draw(gAssets.GlowSprite, t + Scale2D(Range / 128.f), Vec4F{CenterColor, 0.2f}, Blending::Additive);
	r.draw(gAssets.GlowSprite, t + Scale2D(Range / 3 / 128.f), Vec4F{HighlightColor, 0.4f}, Blending::Additive);
	
	Angle phaseRotation[4];
	
	for(int i = 0; i < 4; ++i)
		phaseRotation[i] = TimeSinceSpawn * (0.08_turn + 0.02_turn * i) + (0.99_turn / 3 * i);
	
	for(int i = 0; i < 4; ++i)
		r.draw(Sprite, t + Rotate2D(phaseRotation[i]) + Scale2D(CenterSize), Vec4F{CenterColor, Min(phase[i]*2, 1.0f)});
	
	for(int i = 0; i < 4; ++i)
		r.draw(HighlightSprite, t + Rotate2D(phaseRotation[i]) + Scale2D(CenterSize), Vec4F{HighlightColor, Min(phase[i]/3, 1.0f)});
	
	for(int i = 0; i < 4; ++i)
		r.draw(HighlightSprite2, t + Rotate2D(phaseRotation[i]) + Scale2D(CenterSize), Vec4F{Colors::White, Min(phase[i]/2, 1.0f)}, Blending::Additive);
}

void GravitySource::influence(PhysicsObject* obj, float dt)
{
	Vec2F difference = (Position - obj->Position);
	float distance = difference.length();
	Vec2F direction = difference.normalized();
	obj->Speed += ((direction * Gravity * obj->Mass) / Max((distance/Range)*(distance/Range), 1.0f))*dt;
}

