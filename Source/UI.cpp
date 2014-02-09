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
	for (int i = 0; i < Space->Enemies.UsedLength; ++i)
	{
		Enemy *TheEnemy = Space->Enemies[i];
		Vec2F EnemyPosition = TheEnemy->Position;
		
		Rect<float> Screen = Rect<float>(Space->CameraPos, Space->ScreenSize);
		
		// Only draw the arrow if the enemy is on screen
		//if (Screen.isInside(TheEnemy->Position))
		{		
			//std::cout << Space->CameraPos.X << "|" << Space->CameraPos.Y << " ... " << TheEnemy->Position.X << "|" << TheEnemy->Position.Y << std::endl;
			Vec2F Direction = TheEnemy->Position - Space->ThePlayer->Position;
			//float Length = Direction.getLength();
			Vec2F Normalized = Direction.normalized();
			
			Context->Offset = (Space->ScreenSize / 2) + Normalized * (Space->ScreenSize.Y / 2.2f);
			Context->Rotation = Direction.getAngle();
			Context->setColor(ColorRGB(1, 1, 1), 0.3f);
			ArrowImage.draw(*Context);
		}
	}
}