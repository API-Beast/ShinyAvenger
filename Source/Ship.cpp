//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Ship.h"
#include "Behavior.h"

void Ship::update(float t, PlaySpace* space)
{	
	if (Behavior != NULL)
	{
		Behavior->update(t, this);
	}	
	
	TheMovement->update(t, this, space);
}

void Ship::draw(RenderContext r)
{
	r.setColor(FractionColor, 1.f);
	r.Offset = Position;
	r.Rotation = Rotation;
	Sprite.draw(r);
}