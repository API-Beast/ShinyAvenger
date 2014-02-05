//  Copyright (C) 2014 Beast <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <Springbok/Platform.h>
#include <Springbok/Geometry/Angle.h>
#include "PlaySpace.h"

class Game
{
public:
	Game(int argc, char **argv);
	~Game();
	void execute();
public:
	GameSurface*  Surface;
	InputMonitor* Input;
	PreciseClock* Clock;
	PlaySpace*    Playfield;
};