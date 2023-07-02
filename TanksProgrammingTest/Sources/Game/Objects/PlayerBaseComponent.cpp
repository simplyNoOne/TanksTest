#include "PlayerBaseComponent.h"

#include "../../Engine/Components/CollisionComponent.h"

#include "../../Engine/GameManager.h"

#include "../GameState.h"

#include "Engine.h"
#include "Entity.h"

PlayerBaseComponent::PlayerBaseComponent(Entity* Owner)
	:EntityComponent(Owner)
	, m_WasHit(false)
	, m_WallType(EWallType::BREAKABLE)
	, f_BaseDestroyed(nullptr)
{
}

PlayerBaseComponent::PlayerBaseComponent()
	:PlayerBaseComponent(nullptr)
{
}

void PlayerBaseComponent::LoadFromConfig(const nlohmann::json& Config)
{
}

void PlayerBaseComponent::Initialize()
{
	GameManager::Get()->RegisterSelfAsBase(GetOwner());

	CollisionComponent* CollisionComponentPtr = GetOwner()->GetComponent<CollisionComponent>();

	if (GameManager::Get()->GetBaseID(GetOwner()) == EBase::BASE1)
	{
		CollisionComponentPtr->SetCollisionMask(ECollisionMask::BASE1);
	}
	else if (GameManager::Get()->GetBaseID(GetOwner()) == EBase::BASE2)
	{
		CollisionComponentPtr->SetCollisionMask(ECollisionMask::BASE2);
	}
	CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER1);
	CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER2);
	CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::ENEMY);
	CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PROJECTILE);

	CollisionComponentPtr->BindOnOverlapBegin([this](CollisionComponent* Self, CollisionComponent* Other) {BeginOverlap(Self, Other); });
}

void PlayerBaseComponent::BeginOverlap(CollisionComponent* Self, CollisionComponent* Other)
{
	if (Other->GetCollisionMask() == ECollisionMask::PROJECTILE)
		BaseHit();
}


void PlayerBaseComponent::BaseHit()
{
	if (m_WasHit)
	{
		return;
	}

	m_WasHit = true;

	if (f_BaseDestroyed)
	{
		f_BaseDestroyed();
	}

	Engine::Get()->DeregisterFromCollision(GetOwner()->GetComponent<CollisionComponent>());
	Engine::Get()->RemoveFromActiveScene(GetOwner());
	GameManager::Get()->LoseGame(GetOwner());
	GetOwner()->UnInitialize();

}
