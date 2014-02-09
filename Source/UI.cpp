#include "UI.h"
#include "PlaySpace.h"
#include <iostream>

void EnemyArrow::update(float Time)
{
	
}

void EnemyArrow::draw(RenderContext* Context)
{
	Context->Rotation = Rotation;
	ArrowImage.draw(*Context);
}