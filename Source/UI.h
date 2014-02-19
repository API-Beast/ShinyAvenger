//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "AssetDefinition.h"

#include <Springbok/Geometry/Vec2.h>
#include <Springbok/Containers/List.h>
#include <Springbok/Graphics/Image.h>
#include <Springbok/Graphics/RenderContext.h>
#include <string>

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
	
        void render(RenderContext* Context) 
	{		
		draw(Context);
		
		for (int i = 0; i < Children.UsedLength; ++i)
		{	
			Context->Alignment = 0.f;
			Context->Rotation = 0_turn;
			Context->Offset = Position + Children[i]->Position;
			Children[i]->render(Context);
		}
	}
	
	virtual void draw(RenderContext* Context) { }
	
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

class Arrows : public UIContainer
{
private:
	Angle Rotation;
	PlaySpace *Space;
	Image ArrowImage = Image("UI/Arrow.png");
public:
	Arrows(PlaySpace *TheSpace) : Space(TheSpace) { 
		Size = ArrowImage.getSize();
	}
	
	void draw(RenderContext *Context);
};

class Label : public UIContainer
{
private:
	BitmapFont *Font = &gAssets.SmallFont;
public:
	std::string Text = "";
	ColorRGB Color = Colors::White;
	float Scale = 1.f;
	
	Label() { }
	
	Label(std::string const &text) : Text(text) { }
	
	void draw(RenderContext *Context);
};

class ShieldBar : public UIContainer
{
	void draw(RenderContext *Context);
};