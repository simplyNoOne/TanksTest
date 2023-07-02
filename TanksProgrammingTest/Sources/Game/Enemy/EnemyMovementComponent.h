#pragma once

#include "../../Engine/Components/MovementComponent.h"

class TextureComponent;
class EnemyActionsComponent;
class RootComponent;

class EnemyMovementComponent : public MovementComponent
{

public:
	EnemyMovementComponent(Entity* Owner);
	EnemyMovementComponent();

	virtual EntityComponent* Clone() const override { return new EnemyMovementComponent(*this); }

	virtual void Initialize() override;
	virtual void Update(float DeltaTime) override;

	void ObjectHit(CollisionComponent* Self, CollisionComponent* Other, ECollisionSide Side);

	void AreasEndOverlap(CollisionComponent* Self, CollisionComponent* Other);

	void Turn();
	void TurnInDirection(EMovementDirection Direction);

private:

	EMovementDirection GetNewOptimalDirection();

	EMovementDirection GetRandomDirection();
	EMovementDirection CalculateDirectionToTarget();

private:
	const float TURN_COOLDOWN = 0.5f;
	const int VEL = 80;

	TextureComponent* m_TextureComponentPtr;
	EnemyActionsComponent* m_EnemyActionsComponentPtr;
	RootComponent* m_RootComponentPtr;

	float m_MidTurnElapsed;
};