#pragma once

#include "../../Engine/Components/EntityComponent.h"
#include "../../Engine/EnumInfo.h"

class CollisionComponent;
class MovementComponent;


typedef std::function<void()> PlayerDeadCallback;

class PlayerActionsComponent : public EntityComponent
{

public:
	PlayerActionsComponent(Entity* Owner);
	PlayerActionsComponent();

	virtual EntityComponent* Clone() const override { return new PlayerActionsComponent(*this); }

	virtual void Initialize() override;
	virtual void Update(float DeltaTime) override;

	void BeginOverlap(CollisionComponent* Self, CollisionComponent* other);
	void Hit();

	void BindToPlayerDead(PlayerDeadCallback Call) { f_PlayerDead = Call; }

private:
	void Shoot();

	void EnemyHit();
	void WallDestroyed();

	void Die();


private:
	const float SHOOT_COOLDOWN = 0.4f;

	CollisionComponent* m_CollisionComponentPtr;
	MovementComponent* m_MovementComponentPtr;

	EPlayer m_PlayerID;
	int m_Health;
	int m_EnemiesHit;

	float m_MidShotsElapsed;
	bool m_CanShoot;

	PlayerDeadCallback f_PlayerDead;
};