#include "EnemyActionsComponent.h"

#include "../../Engine/Components/CollisionComponent.h"
#include "../../Engine/Components/RootComponent.h"
#include "../../Engine/ResourceManager.h"
#include "../../Engine/GameManager.h"

#include "../Objects/ProjectileComponent.h"
#include "../Objects/WallComponent.h"
#include "../Objects/PlayerBaseComponent.h"

#include "../Player/PlayerActionsComponent.h"

#include "EnemyMovementComponent.h"

#include "ObjectPool.h"
#include "Engine.h"
#include "Entity.h"

#include <SDL.h>
#include <stdlib.h>   
#include <time.h>   
#include <array>


EnemyActionsComponent::EnemyActionsComponent(Entity* Owner)
	:EntityComponent(Owner)
	, m_CollisionComponentPtr(nullptr)
	, m_EnemyMovementComponentPtr(nullptr)
	, m_Target(EEnemyTarget::PLAYER1)
	, m_DelayTurn(false)
	, m_DestroyedTarget(false)
	, m_DelayTurnElapsed(0.f)
	, m_DelayTurnDirection(EMovementDirection::STATIC)
	, m_MidShotElapsed(0.f)
	, m_NumObjectsInArea({ 0,0,0,0 })
	, m_CollisionAreas({ nullptr, nullptr, nullptr, nullptr })
{
}

EnemyActionsComponent::EnemyActionsComponent()
	:EnemyActionsComponent(nullptr)
{
}

void EnemyActionsComponent::Initialize()
{
	m_CollisionComponentPtr = GetOwner()->GetComponent<CollisionComponent>();
	m_EnemyMovementComponentPtr = GetOwner()->GetComponent<EnemyMovementComponent>();

	m_CollisionComponentPtr->BindOnOverlapBegin([this](CollisionComponent* Self, CollisionComponent* Other) {BeginOverlap(Self, Other); });
	m_CollisionComponentPtr->BindOnHit([this](CollisionComponent* Self, CollisionComponent* Other, ECollisionSide Side) {m_EnemyMovementComponentPtr->ObjectHit(Self, Other, Side); });
	m_CollisionComponentPtr->BindOnHit([this](CollisionComponent* Self, CollisionComponent* Other, ECollisionSide Side) {ObjectHit(Self, Other, Side); });


	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::ENEMY);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::OBSTACLE);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PROJECTILE);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER1);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER2);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::BASE1);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::BASE2);

	srand(time(NULL));


}


void EnemyActionsComponent::Update(float DeltaTime)
{
	m_MidShotElapsed += DeltaTime;
	for (auto Area : m_CollisionAreas)
	{
		Area->SetPosition(GetOwner()->GetComponent<RootComponent>()->GetX(), GetOwner()->GetComponent<RootComponent>()->GetY());
	}

	if (m_DelayTurn)
	{
		m_DelayTurnElapsed += DeltaTime;
		if (m_DelayTurnElapsed > DELAY_TURN_TIME)
		{
			m_DelayTurn = false;
			m_DelayTurnElapsed = 0.f;
			m_EnemyMovementComponentPtr->TurnInDirection(m_DelayTurnDirection);
			Shoot();
			m_EnemyMovementComponentPtr->Turn();
		}
	}

}


void EnemyActionsComponent::BeginOverlap(CollisionComponent* Self, CollisionComponent* Other)
{
	if (Other->GetCollisionMask() == ECollisionMask::PROJECTILE) {
		EnemyHit();
	}
}

void EnemyActionsComponent::ObjectHit(CollisionComponent* Self, CollisionComponent* Other, ECollisionSide Side)
{
	if (Other->GetCollisionMask() != ECollisionMask::ENEMY)
	{
		if (Other->DirectionTowardSide(Side) == m_EnemyMovementComponentPtr->GetMovementDirection())
		{
			Shoot();
		}
	}

}

void EnemyActionsComponent::AreasBeginOverlap(CollisionComponent* Self, CollisionComponent* Other)
{
	int AreaID = GetCollisionAreaID(Self);
	if (Other->GetCollisionMask() == ECollisionMask::OBSTACLE)
	{
		m_NumObjectsInArea[AreaID]++;

		WallComponent* WallComponentPtr = Other->GetOwner()->GetComponent<WallComponent>();
		if (WallComponentPtr)
		{
			if (WallComponentPtr->GetType() == EWallType::BREAKABLE)
			{
				if (static_cast<EMovementDirection>(AreaID) == m_EnemyMovementComponentPtr->GetMovementDirection())
				{
					Shoot();
				}
			}
		}
	}
	else if (Other->GetCollisionMask() != ECollisionMask::ENEMY)
	{
		DelayTurnAndShoot(static_cast<EMovementDirection>(AreaID));
	}
}

void EnemyActionsComponent::AreasEndOverlap(CollisionComponent* Self, CollisionComponent* Other)
{
	if (Other->GetCollisionMask() == ECollisionMask::OBSTACLE)
	{
		m_NumObjectsInArea[GetCollisionAreaID(Self)]--;
	}
}


void EnemyActionsComponent::Shoot()
{
	if (m_MidShotElapsed < SHOOT_COOLDOWN)
	{
		return;
	}
	Entity* Projectile = ObjectPool::Get()->GetProjectile();
	if (!Projectile)
	{
		return;
	}
	Projectile->GetComponent<CollisionComponent>()->RemoveCollisonFromActiveMask(ECollisionMask::ENEMY);
	Projectile->GetComponent<ProjectileComponent>()->Setup(GetOwner());
	m_MidShotElapsed = 0;
}

void EnemyActionsComponent::Setup(int PosX, int PosY)
{
	GetOwner()->GetComponent<RootComponent>()->SetEntityPosition(PosX, PosY);

	PrepareCollisionAreas();
	Engine::Get()->AddToActiveScene(GetOwner());
	Engine::Get()->RegisterForCollision(GetOwner()->GetComponent<CollisionComponent>());

	GetOwner()->GetComponent<MovementComponent>()->SetMovementDirection(EMovementDirection::DOWN);
	m_EnemyMovementComponentPtr->SetCanMove(true);

	int NumOptions = (GameManager::Get()->GetGameMode() == EGameMode::SINGLEPLAYER) ? 2 : 4;
	m_Target = static_cast<EEnemyTarget>(rand() % NumOptions);

	Entity* TargetEntity = GameManager::Get()->GetTarget(m_Target);
	if (m_Target == EEnemyTarget::PLAYER1 || m_Target == EEnemyTarget::PLAYER2)
	{
		TargetEntity->GetComponent<PlayerActionsComponent>()->BindToPlayerDead([this]() {TargetDestroyed(); });
	}
	else
	{
		TargetEntity->GetComponent<PlayerBaseComponent>()->BindToBaseDestroyed([this]() {TargetDestroyed(); });
	}

	m_EnemyMovementComponentPtr->Turn();
}


void EnemyActionsComponent::EnemyHit()
{

	Engine::Get()->DeregisterFromCollision(m_CollisionComponentPtr);
	Engine::Get()->RemoveFromActiveScene(GetOwner());
	GetOwner()->UnInitialize();
	DisposeCollisionAreas();

}

int EnemyActionsComponent::GetCollisionAreaID(CollisionComponent* Area)
{
	for (int i = 0; i < m_CollisionAreas.size(); i++)
	{
		if (m_CollisionAreas[i] == Area) {
			return i;
		}
	}
	return -1;
}


void EnemyActionsComponent::PrepareCollisionAreas()
{
	const EntityComponent* ComponentPrototype = Engine::Get()->GetResourceManager()->GetComponentPrototypeByName("CollisionComponent");
	SDL_Rect* EnemyBounds = GetOwner()->GetComponent<CollisionComponent>()->GetBounds();
	int Unit = 33;
	for (int i = 0; i < NUM_AREAS; i++) {

		CollisionComponent* NewCollisionComponent = (CollisionComponent*)ComponentPrototype->Clone();
		NewCollisionComponent->Initialize();
		NewCollisionComponent->SetSize(Unit, Unit);

		NewCollisionComponent->SetCollisionResponse(ECollisionResponseType::OVERLAP);
		NewCollisionComponent->SetCollisionMask(ECollisionMask::ENEMY);

		NewCollisionComponent->AddCollisonToActiveMask(ECollisionMask::OBSTACLE);
		NewCollisionComponent->AddCollisonToActiveMask(ECollisionMask::ENEMY);
		NewCollisionComponent->AddCollisonToActiveMask(ECollisionMask::PLAYER1);
		NewCollisionComponent->AddCollisonToActiveMask(ECollisionMask::PLAYER2);
		NewCollisionComponent->AddCollisonToActiveMask(ECollisionMask::BASE1);
		NewCollisionComponent->AddCollisonToActiveMask(ECollisionMask::BASE2);

		NewCollisionComponent->BindOnOverlapBegin([this](CollisionComponent* Self, CollisionComponent* Other) {AreasBeginOverlap(Self, Other); });
		NewCollisionComponent->BindOnOverlapEnd([this](CollisionComponent* Self, CollisionComponent* Other) {AreasEndOverlap(Self, Other); });
		NewCollisionComponent->BindOnOverlapEnd([this](CollisionComponent* Self, CollisionComponent* Other) {m_EnemyMovementComponentPtr->AreasEndOverlap(Self, Other); });
		NewCollisionComponent->SetOwner(GetOwner());
		m_CollisionAreas[i] = NewCollisionComponent;
		Engine::Get()->RegisterForCollision(NewCollisionComponent);

	}
	SetCollisionsRelativePositions(8, 10);
}

void EnemyActionsComponent::SetCollisionsRelativePositions(int PosX, int PosY)
{
	m_CollisionComponentPtr->SetRelative(PosX, PosY);
	m_CollisionAreas[0]->SetRelative(PosX + 1, PosY - (AREA_SIZE + 1));
	m_CollisionAreas[1]->SetRelative(PosX + 1, PosY + m_CollisionComponentPtr->GetBounds()->h + 1);
	m_CollisionAreas[2]->SetRelative(PosX - (AREA_SIZE + 1), PosY + 1);
	m_CollisionAreas[3]->SetRelative(PosX + m_CollisionComponentPtr->GetBounds()->w + 1, PosY + 1);

}

void EnemyActionsComponent::DisposeCollisionAreas()
{
	for (int i = 0; i < NUM_AREAS; i++) {
		CollisionComponent* CollisionComponentPtr = m_CollisionAreas[i];
		Engine::Get()->DeregisterFromCollision(CollisionComponentPtr);
		CollisionComponentPtr->UnInitialize();
	}
}

void EnemyActionsComponent::TargetDestroyed()
{
	m_DestroyedTarget = true;
}


void EnemyActionsComponent::DelayTurnAndShoot(EMovementDirection Direction)
{
	m_DelayTurnDirection = Direction;
	m_DelayTurnElapsed = 0.f;
	m_DelayTurn = true;
}