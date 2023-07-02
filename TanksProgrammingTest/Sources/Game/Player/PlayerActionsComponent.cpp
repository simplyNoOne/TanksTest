#include "PlayerActionsComponent.h"

#include "../../Engine/Components/CollisionComponent.h"
#include "../../Engine/Components/EntityComponent.h"
#include "../../Engine/Components/MovementComponent.h"
#include "../../Engine/GameManager.h"

#include "../GameState.h"

#include "../Objects/ProjectileComponent.h"

#include "Entity.h"
#include "PlayerInputComponent.h"
#include "ObjectPool.h"

#include <SDL.h>

PlayerActionsComponent::PlayerActionsComponent(Entity* Owner)
	:EntityComponent(Owner)
	, m_CollisionComponentPtr(nullptr)
	, m_MovementComponentPtr(nullptr)
	, m_PlayerID(EPlayer::PLAYER1)
	, m_Health(GameState::PLAYER_MAX_HEALTH)
	, m_EnemiesHit(0)
	, m_CanShoot(true)
	, m_MidShotsElapsed(0.f)
{
}

PlayerActionsComponent::PlayerActionsComponent()
	:PlayerActionsComponent(nullptr)
{
}

void PlayerActionsComponent::Initialize()
{
	GameManager::Get()->RegisterSelfAsPlayer(GetOwner());
	m_PlayerID = GameManager::Get()->GetPlayerID(GetOwner());

	m_MovementComponentPtr = GetOwner()->GetComponent<MovementComponent>();
	m_CollisionComponentPtr = GetOwner()->GetComponent<CollisionComponent>();

	if (m_PlayerID == EPlayer::PLAYER1)
	{
		m_CollisionComponentPtr->SetCollisionMask(ECollisionMask::PLAYER1);
		m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER2);
	}
	else if (GameManager::Get()->GetPlayerID(GetOwner()) == EPlayer::PLAYER2)
	{
		m_CollisionComponentPtr->SetCollisionMask(ECollisionMask::PLAYER2);
		m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER1);
	}

	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::OBSTACLE);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::ENEMY);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PROJECTILE);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::BASE1);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::BASE2);

	m_CollisionComponentPtr->BindOnOverlapBegin([this](CollisionComponent* Self, CollisionComponent* Other) {BeginOverlap(Self, Other); });

	PlayerInputComponent* PlayerInputComponentPtr = GetOwner()->GetComponent<PlayerInputComponent>();
	if (PlayerInputComponentPtr)
	{
		if (m_PlayerID == EPlayer::PLAYER1)
		{
			PlayerInputComponentPtr->BindActionShoot1([this]() {Shoot(); });
			if (GameManager::Get()->GetGameMode() == EGameMode::SINGLEPLAYER)		//in case of singleplayer, player has two sets of controls
			{
				PlayerInputComponentPtr->BindActionShoot2([this]() {Shoot(); });
			}
		}
		else if (m_PlayerID == EPlayer::PLAYER2)
		{
			PlayerInputComponentPtr->BindActionShoot2([this]() {Shoot(); });
		}

	}

}

void PlayerActionsComponent::Update(float DeltaTime)
{
	m_MidShotsElapsed += DeltaTime;
	if (!m_CanShoot && m_MidShotsElapsed > SHOOT_COOLDOWN)
	{
		m_CanShoot = true;
	}

}

void PlayerActionsComponent::BeginOverlap(CollisionComponent* Self, CollisionComponent* Other)
{
	if (Other->GetCollisionMask() == ECollisionMask::PROJECTILE) {
		Hit();
	}
}

void PlayerActionsComponent::Hit()
{
	m_Health -= GameState::PLAYER_HEALTH_DECREMENT;
	if (m_Health <= 0)
	{
		if (f_PlayerDead)
		{
			f_PlayerDead();
		}
		Die();
	}
}

void PlayerActionsComponent::Shoot()
{
	if (!m_CanShoot)
	{
		return;
	}
	Entity* Projectile = ObjectPool::Get()->GetProjectile();
	if (!Projectile)
	{
		return;
	}

	m_MidShotsElapsed = 0.f;
	m_CanShoot = false;

	Projectile->GetComponent<CollisionComponent>()->RemoveCollisonFromActiveMask(m_CollisionComponentPtr->GetCollisionMask());
	if (m_PlayerID == EPlayer::PLAYER1)
	{
		Projectile->GetComponent<CollisionComponent>()->RemoveCollisonFromActiveMask(ECollisionMask::BASE1);
	}
	else if (m_PlayerID == EPlayer::PLAYER2)
	{
		Projectile->GetComponent<CollisionComponent>()->RemoveCollisonFromActiveMask(ECollisionMask::BASE2);
	}

	Projectile->GetComponent<ProjectileComponent>()->BindToEnemyHit([this]() {EnemyHit(); });
	Projectile->GetComponent<ProjectileComponent>()->BindToWallDestroyed([this]() {WallDestroyed(); });
	Projectile->GetComponent<ProjectileComponent>()->Setup(GetOwner());

}

void PlayerActionsComponent::EnemyHit()
{

	m_EnemiesHit++;

	//if we were currently colliding with this enemy and couldn't move, we can move now
	if (!m_MovementComponentPtr->GetCanMove())
	{
		m_MovementComponentPtr->SetCanMove(true);
	}
	if (m_EnemiesHit >= GameState::NUMBER_ENEMIES)
	{
		GameManager::Get()->WinGame();
	}
}

void PlayerActionsComponent::WallDestroyed()
{
	//if we were currently colliding with this wall and couldn't move, we can move now
	if (!m_MovementComponentPtr->GetCanMove())
	{
		m_MovementComponentPtr->SetCanMove(true);
	}

}


void PlayerActionsComponent::Die()
{
	Engine::Get()->DeregisterFromCollision(m_CollisionComponentPtr);
	Engine::Get()->RemoveFromActiveScene(GetOwner());
	GameManager::Get()->LoseGame(GetOwner());
	GetOwner()->UnInitialize();
}

