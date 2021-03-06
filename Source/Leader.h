//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <Springbok/Geometry/Vec2.h>

class Leader
{
public:
	List<Ship*> Group;
	virtual Vec2F getPosition() const = 0;
	virtual bool hasMoved() const = 0;
	virtual bool isDestroyed() const = 0;
};