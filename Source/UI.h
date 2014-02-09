//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <Springbok/Geometry/Vec2.h>
#include <Springbok/Containers/List.h>
#include <Springbok/Graphics/Image.h>
#include <Springbok/Graphics/RenderContext.h>

class Enemy;
class PlaySpace;

class UIContainer
{	
protected:	
	List<UIContainer*> Children;
public:		
	Vec2F Position;
	Vec2F Size;
	bool Visible = true;
	
	virtual void update(float Time) 
	{ 
		for (int i = 0; i < Children.UsedLength; ++i)
		{	
			Children[i]->update(Time);
		}
	}
        virtual void draw(RenderContext* Context) 
	{
		Context->CameraPos = Position;
	
		for (int i = 0; i < Children.UsedLength; ++i)
		{	
			if (Children[i]->Visible)
			{
				Context->Offset = Children[i]->Position;
				Children[i]->draw(Context);
			}
		}
	}
	
	float left() { return Position.X; }
	float top() { return Position.Y; }
	float right() { return Position.X + Size.X; }
	float bottom() { return Position.Y + Size.Y; }
	
	void append(UIContainer* Child) { Children.pushBack(Child); }
	void remove(UIContainer* Child) { 		
		for (int i = 0; i < Children.UsedLength; ++i)
		{
			if (Child == Children[i])
			{
				Children.quickRemove(i);
				break;
			}
		}
	}
};

class EnemyArrow : public UIContainer
{
private:
	Angle Rotation;
	Enemy *TheEnemy;
	PlaySpace *Space;
	Image ArrowImage = Image("UI/Arrow.png");
public:
	EnemyArrow(Enemy* TheEnemy, PlaySpace *TheSpace) : TheEnemy(TheEnemy), Space(TheSpace) { 
		Size = ArrowImage.getSize();
	}
	
	void draw(RenderContext *Context);	
	void update(float Time);
};