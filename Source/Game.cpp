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
		float dt = 0.0f;
		dt = Clock->elapsed();
		Clock->start();
		
		for(const ButtonPressEvent& press : Input->poll())
		{
			if(Buttons::Escape.matches(press)) // Escape was pressed
			{
				Surface->requestClose();
				continue;
			}
		}
		bool up    = Buttons::Up.isPressed(Input);
		bool down  = Buttons::Down.isPressed(Input);
		bool right = Buttons::Right.isPressed(Input);
		bool left  = Buttons::Left.isPressed(Input);
		
		if(up || down || right || left)
			Playfield->onMovementInput(Angle::FromBooleanDirectionMatrix(up, down, right, left), dt);
		
		if(Input->getPrimaryPointerDevice()->anyButtonPressed())
			Playfield->onMouseHoldInput(Input->getPrimaryPointerDevice()->getCursorPosition(0));
		
		Playfield->update(dt);
		Playfield->draw();
		
		Surface->switchBuffers();
	}
	return;
}
