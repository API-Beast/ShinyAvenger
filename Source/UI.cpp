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
	Context->Rotation = Rotation;
	ArrowImage.draw(*Context);
}