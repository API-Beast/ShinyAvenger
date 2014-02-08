//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <Springbok/Geometry/Angle.h>
#include <Springbok/Graphics/Image.h>
#include "PhysicsObject.h"

class EnemyBehavior;

class Enemy : public PhysicsObject
{
public:
      Angle MovementDirection = 0;
      Angle TargetDirection = 0;
      float AcclerateFactor = 0.0f;
private:
      Image Sprite;
      EnemyBehavior *Behavior;
public:
      Enemy(EnemyBehavior* b = NULL) : Sprite("Enemy/Enemy01.png"), Behavior(b) {};
      void update(float t);
      virtual void draw(RenderContext r);
};
