//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "UI.h"
#include "PlaySpace.h"
#include <iostream>

void EnemyArrow::draw(RenderContext* Context)
{
	for(Ship* ship : Space->Ships)
	{
		Vec2F EnemyPosition = ship->Position;
		Vec2F Direction = ship->Position - Space->Player->Position;
		float Length = Direction.getLength();
		Vec2F Normalized = Direction.normalized();
			
		Context->Offset = (Space->ScreenSize / 2) + Normalized * ((Space->ScreenSize.Y / 3.2f) + (20000.0f / (Length / 5)));
		Context->Rotation = Angle(Direction);
		float Opacity = 150.0 / Length;
			
		if (Length < Space->ScreenSize.X / 1.5f) {
			Opacity = Length / 10000.0f;
		}
			
		Context->setColor(ship->FactionColor, Opacity);
		ArrowImage.draw(*Context);
		
	}
}