#pragma once
#include "Actor.h"
class Orc : public Actor, public ICanBattle
{
public:
	Orc() :Actor("오크", 50.0f, 10.0f) {}
	Orc(const char* InName) :Actor(InName, 50.0f, 10.0f) {}
	Orc(const char* InName, float InHealth, float InAttackPower) : Actor(InName, InHealth, InAttackPower) {}

	/// <summary>
	/// Target에게 공격하는 함수
	/// </summary>
	/// <param name="InTarget">공격 받는 대상</param>
	virtual void ApplyDamage(ICanBattle* InTarget) override;

	/// <summary>
	/// 지정된 피해량만큼 본인에게 피해를 적용
	/// </summary>
	/// <param name="InDamage">적용할 피해량</param>
	virtual void TakeDamage(float InDamage) override;

	inline int GetDropGold() const { return DropGold; }

	virtual ~Orc() {};

protected:
	int DropGold = 30;
};

