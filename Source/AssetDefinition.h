//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once
#include "Particle.h"

struct AssetDefinition
{
	void initAll();
	
	Particle::_Definition EnergyShield;
	Particle::_Definition PulseEngineGlow;
	Particle::_Definition PulseEngineSpark;
};

extern AssetDefinition gAssets;