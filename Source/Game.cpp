//  Copyright (C) 2014 Beast <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Game.h"
#include <Springbok/Graphics/RenderContext.h>

Game::Game(int argc, char** argv)
{
	Surface = new GameSurface("ShinyAvenger", GameSurface::Windowed);
	Input   = new InputMonitor(Surface);
	Clock   = new PreciseClock();
	Playfield = new PlaySpace;
	RenderContext::Setup2DEnvironment();
}

Game::~Game()
{
	delete Surface;
	delete Input;
	delete Clock;
	delete Playfield;
}

void Game::execute()
{
	Clock->start();
	while(!Surface->closeRequested())
	{
		float dt = Clock->elapsed();
		Clock->start();
		
		for(const ButtonPressEvent& press : Input->poll())
		{
			if(Buttons::Escape.matches(press)) // Escape was pressed
				Surface->requestClose();
			if(Buttons::Up.matches(press))
				Playfield->onMovementInput(0.00f);
			else if(Buttons::Right.matches(press))
				Playfield->onMovementInput(0.25f);
			else if(Buttons::Down.matches(press))
				Playfield->onMovementInput(0.50f);
			else if(Buttons::Left.matches(press))
				Playfield->onMovementInput(0.75f);
		}
		
		Playfield->update(dt);
		Playfield->draw();
		
		Surface->switchBuffers();
	}
	return;
}
