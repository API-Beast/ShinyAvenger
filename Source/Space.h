//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

class Space
{
public:
	virtual ~Space(){};
	virtual void update(float time){};
	virtual void draw(){};
	virtual bool isFinished(){ return false; };
};