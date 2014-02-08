//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "PlaySpace.h"
#include <GL/gl.h>
#include <Springbok/Animation/Interpolation.h>

PlaySpace::PlaySpace()
{
	ThePlayer = new Player;
	ThePlayer->Position = {200, 150};
	Objects.pushBack(ThePlayer);
	GlowSprite = Image("Glow.png");
	GravitySourceSprite = Image("GravitySource.png");
	GravitySourceHighlight = Image("GravitySourceHighlight.png");
	
	GravitySources.pushBack({Vec2F(0,0), 0.3f, 750.f, ColorRGB(0.62f, 0.2f, 0.44f), ColorRGB(0.92f, 0.5f, 0.44f)});
	
	glClearColor(0.22f, 0.15f, 0.24f, 0.0f);
}

PlaySpace::~PlaySpace()
{
	for(PhysicsObject* obj : Objects)
		delete obj;
}

void PlaySpace::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	RenderContext r2;
	r2.CameraPos = Vec2F(-400, -300) + ThePlayer->Position;
	for(GravitySource& src : GravitySources)
	{
		RenderContext r(r2);
		r.Offset = src.Position;
		
		// Background 1
		r.Rotation = GameTime*0.15f;
		r.Scale = 1.2f;
		r.setColor(Colors::Black, 0.4f);
		GlowSprite.drawStretched(src.Range*2, r);
		
		// Background 2
		r.Rotation = GameTime*0.32f;
		r.Scale = 1.0f;
		r.setColor(Colors::Black, 0.6f);
		GlowSprite.drawStretched(src.Range*2, r);
		
		// Background Center 1
		r.Rotation = GameTime*0.21f;
		r.Scale = 1.6f;
		r.setColor(src.CenterColor, 0.7f);
		GravitySourceSprite.draw(r);
		
		// Background Center 2
		r.Rotation = GameTime*0.36f;
		r.Scale = 1.1f;
		r.setColor(src.CenterColor);
		GravitySourceSprite.draw(r);
		
		// Background Center Highlight
		r.Rotation = GameTime*0.40f;
		r.Scale = 1.0f;
		r.setColor(src.HighlightColor);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		GravitySourceHighlight.draw(r);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	r2.setColor(Colors::White);
	for(PhysicsObject* obj : Objects)
		obj->draw(r2);
}

void PlaySpace::update(float time)
{
	GameTime += time;
	
	
	ThePlayer->update(time);
	for(PhysicsObject* obj : Objects)
	{
		obj->Speed    += obj->Acceleration * time;
		obj->Speed    -= obj->Speed * (AirDrag * obj->Drag * time) * obj->Speed.getLength();
		
		// Add gravity
		for(GravitySource& src : GravitySources)
		{
			Vec2F difference = (src.Position - obj->Position);
			float distance = difference.getLength();
			Vec2F direction = difference.normalized();
			obj->Speed += (direction * src.Gravity * obj->Mass) / Max((distance/src.Range)*(distance/src.Range),1.0f);
		}
		
		obj->Position += obj->Speed * time;
	}
}

void PlaySpace::onMovementInput(Angle angle, float time)
{
	ThePlayer->TargetDirection = Approach(ThePlayer->TargetDirection, angle, time);
	ThePlayer->AcclerateFactor = 1.0f;
}
