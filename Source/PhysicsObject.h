//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <Springbok/Geometry/Vec2.h>
#include <Springbok/Geometry/BoundingShapes.h>
#include <Springbok/Graphics/SpriteRenderer.h>

class PlaySpace;

struct PhysicsObject
{
	Vec2F Position = 0;
	Vec2F Speed = 0;
	Vec2F Acceleration = 0;
	Vec2F Drag = 1;
	Vec2F Mass = 1;
	
	float Flow = 1;
	float Stabilizer = 0;
	float NegativeForce = 0;
	
	Angle Rotation = 0_turn;
	float RotationSpeed = 0.0f;
	
	Vec2F BoundsOffset = 0;
	BoundingRect Bounds = BoundingRect{0, 16}; // Position updated every frame
public:
	virtual void draw(SpriteRenderer& r){};
	virtual void drawPictogram(SpriteRenderer& r, float sizeMult){};
	virtual void update(float dt, PlaySpace* space){};
	virtual void updateBounds(){ Bounds.setCenter(Position - BoundsOffset); };
	PhysicsObject& operator=(const PhysicsObject& other) = default;
};