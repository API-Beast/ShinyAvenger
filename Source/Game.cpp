//  Copyright (C) 2014 Beast <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Game.h"

Game::Game(int argc, char** argv)
{
	Surface = new GameSurface("ShinyAvenger", GameSurface::Windowed);
	Input   = new InputMonitor(Surface);
	Clock   = new PreciseClock();
}

void Game::execute()
{
	while(!Surface->closeRequested())
	{
		for(const ButtonPressEvent& press : Input->poll())
		{
			if(Buttons::Escape.matches(press)) // Escape was pressed
				Surface->requestClose();
		}
		
		Surface->switchBuffers();
	}
	return;
}
