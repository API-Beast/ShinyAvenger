//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include <iostream>
#include "Source/Game.h"
#include <Springbok/Resources/ResourceManager.h>

int main(int argc, char **argv)
{
	ResourceManager::GetInstance()->findPrimaryResourcePath({"./Assets"});
	Game game(argc, argv);
	game.execute();
	return 0;
}
