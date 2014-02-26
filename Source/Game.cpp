//  Copyright (C) 2014 Beast <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Game.h"
#include <Springbok/Utils/CliArgumentParser.h>
#include <Springbok/Graphics/RenderContext.h>

#include <iostream>

Game::Game(int argc, char** argv)
{
	CliArgumentParser argParser;
	argParser.addSwitch("--fullscreen");
	argParser.addSwitch("--size", 2);
	
	argParser.parse(argc, argv);
	
	if(argParser["--fullscreen"])
		Surface = new GameSurface("ShinyAvenger");
	else if(argParser["--size"])
		Surface = new GameSurface("ShinyAvenger", GameSurface::Windowed, {std::stoi(argParser["--size"][0]), std::stoi(argParser["--size"][1])});
	else
		Surface = new GameSurface("ShinyAvenger", GameSurface::Windowed, {1024, 768});
	Input   = new InputMonitor(Surface);
	Clock   = new PreciseClock();
	Playfield = new PlaySpace(Surface, argParser.LooseArguments);
	RenderContext::Setup2DEnvironment();
}

Game::~Game()
{
	delete Surface;
	delete Input;
	delete Clock;
	delete Playfield;
	std::cout << "Game gets deleted" << std::endl;
}

void Game::execute()
{
	Clock->start();
	while(!Surface->closeRequested())
	{
		float dt = 0.0f;
		//while(Clock->elapsed() < 0.1);
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
		
		bool actionA = Buttons::ActionA.isPressed(Input);
		bool actionB = Buttons::ActionB.isPressed(Input);
		bool actionC = Buttons::ActionC.isPressed(Input);
		
		if(up || down || right || left)
			Playfield->onMovementInput(up, down, right, left, dt);
		
		if(actionA || actionB || actionC)
			Playfield->onActionInput(actionA, actionB, actionC);
		
		if(Input->getPrimaryPointerDevice()->anyButtonPressed())
			Playfield->onMouseHoldInput(Input->getPrimaryPointerDevice()->getCursorPosition(0));
		
		Playfield->update(dt);
		Playfield->draw();
		
		Surface->switchBuffers();
	}
	return;
}
