#pragma once
#include "Actor.h"
#include <random>
class Orc : public Actor
{
public:

	/*const float GetHealth() const { return Health; }
	const float GetAttackPower() const { return AttackPower; }
	const int GetDropGold() const { return DropGold; }*/

	float Health = 70.0f;
	float AttackPower = 10.0f;
	int DropGold = 50;

public:
	Orc() = default;
	Orc(std::string InName)
		: Actor(InName)
	{

	}
protected:
	void SetHealth(float Modifier = 1.0f)
	{
		float RandomFactor = 0.9f + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f);
		Health = Health * Modifier * RandomFactor;
	}
	void SetAttackPower(float Modifier = 1.0f)
	{
		float RandomFactor = 0.9f + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f);
		AttackPower = AttackPower * Modifier * RandomFactor;
	}
	void SetDropGold(float Modifier = 1.0f)
	{
		float RandomFactor = 0.9f + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f);
		DropGold = static_cast<int>(DropGold * Modifier * RandomFactor);
	}



};

