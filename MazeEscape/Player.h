#pragma once
#include "Actor.h"
#include "Position.h"
#include "ICanBattle.h"

class APlayer : public Actor, public ICanBattle
{
public:
	APlayer() : Actor("플레이어", 100.0f, 20.0f) { }
	APlayer(const char* InName, float InHealth, float InAttackPower)
		: Actor(InName, InHealth, InAttackPower)
	{
	}

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

	inline void AddGold(int InGold) { Gold += InGold; }

	virtual ~APlayer() {};

protected:
	Position CurrentPosition = Position(0,0);
	int Gold = 0;
};


