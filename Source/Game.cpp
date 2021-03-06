//  Copyright (C) 2014 Beast <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Game.h"
#include "PlaySpace.h"

#include <Springbok/Utils/CliArgumentParser.h>
#include <Springbok/Graphics/Core/RenderContext2D.h>

#include <Springbok/Platform/GameSurface.h>
#include <Springbok/Platform/InputMonitor.h>
#include <Springbok/Platform/PreciseClock.h>
#include <Springbok/Platform/InputDevice.h>

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
		Surface = new GameSurface("ShinyAvenger", GameSurface::Windowed, {std::stoi(argParser["--size"][0]), std::stoi(argParser["--size"][1])}, 4);
	else
		Surface = new GameSurface("ShinyAvenger", GameSurface::Windowed, {1024, 768}, 4);
	Input   = new InputMonitor(Surface);
	Clock   = new PreciseClock();
	Playfield = new PlaySpace(Surface, argParser.LooseArguments);
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
		bool space   = Buttons::Space.isPressed(Input);
		
		if(up || down || right || left)
			Playfield->onMovementInput(up, down, right, left, dt);
		
		if(actionA || actionB || actionC || space)
			Playfield->onActionInput(actionA, actionB, actionC, space);
		
		if(Input->getPrimaryPointerDevice()->anyButtonPressed())
			Playfield->onMouseHoldInput(Input->getPrimaryPointerDevice()->getCursorPosition(0));
		
		Playfield->onScrollInput(Input->getPrimaryPointerDevice()->getScrollWheelState());
		Input->getPrimaryPointerDevice()->setScrollWheelState(0);
		
		Playfield->update(dt);
		Playfield->draw();
		
		Surface->switchBuffers();
		
		if(Playfield->isFinished())
		{
			delete Playfield;
			Playfield = new PlaySpace(Surface, {});
		}
	}
	return;
}
