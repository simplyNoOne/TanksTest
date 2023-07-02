#pragma once

#include "../../Engine/Components/EntityComponent.h"
#include "../Engine/EnumInfo.h"

class CollisionComponent;
class RootComponent;
class MovementComponent;

typedef std::function<void()> ObjectHitCallback;


class ProjectileComponent : public EntityComponent
{

public:
	ProjectileComponent(Entity* Owner);
	ProjectileComponent();

	virtual EntityComponent* Clone() const override { return new ProjectileComponent(*this); }

	virtual void Initialize() override;


	void BindToEnemyHit(ObjectHitCallback Call) { f_EnemyHit = Call; }
	void BindToWallDestroyed(ObjectHitCallback Call) { f_WallDestroyed = Call; }

	void BeginOverlap(CollisionComponent* Self, CollisionComponent* Other);

	void Setup(Entity* Shooter);

private:
	void Shoot();
	void DestroySelf();

private:
	const int VEL = 500;

	MovementComponent* m_MovementComponentPtr;
	CollisionComponent* m_CollisionComponentPtr;
	RootComponent* m_RootComponentPtr;

	ObjectHitCallback f_EnemyHit;
	ObjectHitCallback f_WallDestroyed;

};