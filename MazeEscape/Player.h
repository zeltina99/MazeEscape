#pragma once
#include "Actor.h"
#include "Position.h"

class Player : public Actor
{
public:
	const float MaxHealth = 100.0f;

	Position CurrentPosition = Position(0, 0);
	float Health = MaxHealth;
	float AttackPower = 20.0f;
	int Gold = 0;




public:
	Player() = default;
	Player(std::string InName)
		: Actor(InName), CurrentPosition(0, 0), AttackPower(20.0f), Health(100.0f), Gold(0)
	{

	}

protected:




};
