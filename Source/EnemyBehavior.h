//  Copyright (C) 2014 Miguel Gonzalez <miguel-gonzalez@gmx.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "PhysicsObject.h"
#include "Enemy.h"

class EnemyBehavior
{
public:
      virtual void update(float t, Enemy* const) = 0;      
};

class TrackingBehavior : public EnemyBehavior
{
public:
      PhysicsObject *Target;
      
      TrackingBehavior(PhysicsObject *PhysicsTarget) : Target(PhysicsTarget) {}
      
      void update(float t, Enemy *const TheEnemy)
      {
          TheEnemy->Position.X = 10;
      }
};

