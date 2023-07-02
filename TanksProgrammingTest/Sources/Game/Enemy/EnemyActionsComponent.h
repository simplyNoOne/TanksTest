#pragma once

#include <iostream>
#include "../../Engine/Components/EntityComponent.h"
#include "../Engine/EnumInfo.h"

class RootComponent;
class CollisionComponent;
class EnemyMovementComponent;
class EnemyActionsComponent : public EntityComponent
{

public:
	EnemyActionsComponent(Entity* Owner);
	EnemyActionsComponent();

	virtual EntityComponent* Clone() const override { return new EnemyActionsComponent(*this); }

	virtual void Initialize() override;
	virtual void Update(float DeltaTime) override;

	void BeginOverlap(CollisionComponent* Self, CollisionComponent* other);
	void ObjectHit(CollisionComponent* Self, CollisionComponent* Other, ECollisionSide Side);

	void AreasBeginOverlap(CollisionComponent* Self, CollisionComponent* Other);
	void AreasEndOverlap(CollisionComponent* Self, CollisionComponent* Other);

	void SetCollisionsRelativePositions(int PosX, int PosY);

	int GetCollisionAreaID(CollisionComponent* Area);
	int GetNumOverlapsInAreaID(int AreaID) { return m_NumObjectsInArea[AreaID]; }
	bool GetDestroyedTarget() { return m_DestroyedTarget; }
	bool GetWaitingToTurn() { return m_DelayTurn; }
	EEnemyTarget GetTarget() { return m_Target; }

	void Setup(int PosX, int PosY);
	void DelayTurnAndShoot(EMovementDirection Direction);

private:
	void Shoot();

	void EnemyHit();
	void TargetDestroyed();

	void PrepareCollisionAreas();
	void DisposeCollisionAreas();


private:

	static const int NUM_AREAS = 4;
	const int AREA_SIZE = 33;
	const float DELAY_TURN_TIME = 0.5f;		//time for enemy to move once it detects a beginning of a collision, so that it doesn't turn too fast
	const float SHOOT_COOLDOWN = 1.f;


	CollisionComponent* m_CollisionComponentPtr;
	EnemyMovementComponent* m_EnemyMovementComponentPtr;

	EEnemyTarget m_Target;

	bool m_DelayTurn;
	float m_DelayTurnElapsed;
	EMovementDirection m_DelayTurnDirection;

	float m_MidShotElapsed;

	bool m_DestroyedTarget;

	std::array<int, NUM_AREAS> m_NumObjectsInArea;
	std::array<CollisionComponent*, NUM_AREAS> m_CollisionAreas;

};