#pragma once
#include "Actor.h"
class Goblin : public Actor, public ICanBattle
{
public:
	Goblin() :Actor("고블린", 20.0f, 5.0f) {}
	Goblin(const char* InName) :Actor(InName, 20.0f, 5.0f) {}
	Goblin(const char* InName, float InHealth, float InAttackPower) : Actor(InName, InHealth, InAttackPower) {}

	/// <summary>
	/// Target에게 공격하는 함수
	/// </summary>
	/// <param name="InTarget">공격 받는 대상</param>
	void ApplyDamage(ICanBattle* InTarget);

	/// <summary>
	/// 지정된 피해량만큼 본인에게 피해를 적용
	/// </summary>
	/// <param name="InDamage">적용할 피해량</param>
	void TakeDamage(float InDamage);

	inline int GetDropGold() const { return DropGold; }

protected:
	int DropGold = 10;
};

