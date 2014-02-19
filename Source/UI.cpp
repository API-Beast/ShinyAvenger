//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "UI.h"
#include "PlaySpace.h"
#include <iostream>

void Arrows::draw(RenderContext* Context)
{
	for(Ship* ship : Space->Ships)
	{
		// Skip the own ship
		if (ship == Space->Player)
			continue;
		
		Vec2F EnemyPosition = ship->Position;
		Vec2F Direction = ship->Position - Space->Player->Position;
		float Length = Direction.getLength();
		Vec2F Normalized = Direction.normalized();
			
		Context->Offset = (Space->ScreenSize / 2) + Normalized * ((Space->ScreenSize.Y / 3.2f) + (20000.0f / (Length / 5)));
		Context->Rotation = Angle(Direction);
		Context->Scale = 200.0 / Length + 0.5f;
		float Opacity = 150.0 / Length;
			
		if (Length < Space->ScreenSize.X / 2.f) {
			Opacity = Length / (Space->ScreenSize.X / 1.3f) - 0.5f;
			Context->Offset = (Space->ScreenSize / 2) + (Direction * 0.8f);
		}
			
		Context->setColor(ship->FactionColor, Opacity);
		ArrowImage.draw(*Context);
		
	}
}

void Label::draw(RenderContext *Context)
{
	Context->Scale = Scale;
	Context->setColor(Color);
	Font->draw(Text, *Context);
}

void ShieldBar::draw(RenderContext *Context)
{
	
}