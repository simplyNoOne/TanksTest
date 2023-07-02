#include "WallComponent.h"

#include "../../Engine/Components/CollisionComponent.h"

#include "Entity.h"
#include "Engine.h"

WallComponent::WallComponent(Entity* Owner)
	:EntityComponent(Owner)
	, m_WallType(EWallType::UNBREAKABLE)
	, m_WasHit(false)
{
}

WallComponent::WallComponent()
	:WallComponent(nullptr)
{
}

void WallComponent::LoadFromConfig(const nlohmann::json& Config)
{
	m_WallType = EWallType(Config.value("WallType", 1));
}

void WallComponent::Initialize()
{
	CollisionComponent* CollisionComponentPtr = GetOwner()->GetComponent<CollisionComponent>();
	if (CollisionComponentPtr) {
		CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER1);
		CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER2);
		CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::ENEMY);
		CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PROJECTILE);

		CollisionComponentPtr->BindOnOverlapBegin([this](CollisionComponent* Self, CollisionComponent* Other) {BeginOverlap(Self, Other); });
	}
}


void WallComponent::BeginOverlap(CollisionComponent* Self, CollisionComponent* Other)
{
	if (Other->GetCollisionMask() == ECollisionMask::PROJECTILE)
	{
		WallShot();
	}
}

void WallComponent::WallShot()
{
	if (m_WasHit)
	{
		return;
	}
	if (m_WallType == EWallType::BREAKABLE)
	{
		Engine::Get()->DeregisterFromCollision(GetOwner()->GetComponent<CollisionComponent>());
		Engine::Get()->RemoveFromActiveScene(GetOwner());
		GetOwner()->UnInitialize();
	}
}