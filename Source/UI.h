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
	UIContainer *parent = nullptr;
public:		
	Vec2F Position;
	Vec2F Size;
	
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
			Context->Offset = Children[i]->Position;
			Children[i]->draw(Context);
		}
	}
	
	float left()   const { return parent != NULL ? parent->left() + Position.X : Position.X; }
	float top()    const { return parent != NULL ? parent->top()  + Position.Y : Position.Y; }
	float right()  const { return left() + Size.X; }
	float bottom() const { return top()  + Size.Y; }
	
	void append(UIContainer* Child) 
	{ 
		if (Child->parent != NULL)
		{
			Child->parent->remove(Child);
		}
		
		Children.pushBack(Child); 
		Child->parent = this;
		
	}
	void remove(UIContainer* Child) { 		
		for (int i = 0; i < Children.UsedLength; ++i)
		{
			if (Child == Children[i])
			{
				Child->parent = NULL;
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
	PlaySpace *Space;
	Image ArrowImage = Image("UI/Arrow.png");
public:
	EnemyArrow(PlaySpace *TheSpace) : Space(TheSpace) { 
		Size = ArrowImage.getSize();
	}
	
	void draw(RenderContext *Context);
};

class ShieldBar : public UIContainer
{
	
};