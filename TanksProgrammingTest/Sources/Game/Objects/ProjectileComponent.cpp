#include "ProjectileComponent.h"

#include "../Engine/ResourceManager.h"

#include "../Engine/Components/CollisionComponent.h"
#include "../Engine/Components/TextureComponent.h"
#include "../Engine/Components/MovementComponent.h"
#include "../Engine/Components/RootComponent.h"

#include "Entity.h"
#include "Engine.h"
#include "ObjectPool.h"
#include "WallComponent.h"

ProjectileComponent::ProjectileComponent(Entity* Owner)
	:EntityComponent(Owner)
	, m_MovementComponentPtr(nullptr)
	, m_CollisionComponentPtr(nullptr)
	, m_RootComponentPtr(nullptr)
	, f_EnemyHit(nullptr)
	, f_WallDestroyed(nullptr)
	
{
}

ProjectileComponent::ProjectileComponent()
	:ProjectileComponent(nullptr)
{
}

void ProjectileComponent::Initialize()
{
	m_MovementComponentPtr = GetOwner()->GetComponent<MovementComponent>();
	m_RootComponentPtr = GetOwner()->GetComponent<RootComponent>();
	m_CollisionComponentPtr = GetOwner()->GetComponent<CollisionComponent>();

	m_CollisionComponentPtr->SetCollisionMask(ECollisionMask::PROJECTILE);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::OBSTACLE);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::ENEMY);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER1);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER2);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::BASE1);
	m_CollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::BASE2);

	m_CollisionComponentPtr->BindOnOverlapBegin([this](CollisionComponent* Self, CollisionComponent* Other) {BeginOverlap(Self, Other); });
}


void ProjectileComponent::BeginOverlap(CollisionComponent* Self, CollisionComponent* Other)
{
	m_MovementComponentPtr->SetCanMove(false);
	if (Other->GetCollisionMask() == ECollisionMask::ENEMY)
	{
		if (f_EnemyHit)
		{
			f_EnemyHit();
			f_EnemyHit = nullptr;
		}
	}
	else if (Other->GetCollisionMask() == ECollisionMask::OBSTACLE)
	{
		WallComponent* WallComponentPtr = Other->GetOwner()->GetComponent<WallComponent>();
		if (WallComponentPtr)
		{
			if (WallComponentPtr->GetType() == EWallType::BREAKABLE)
			{
				if (f_WallDestroyed)
				{
					f_WallDestroyed();
					f_WallDestroyed = nullptr;
				}
			}
		}
	}

	DestroySelf();
	
}

void ProjectileComponent::Setup(Entity* Shooter)
{
	SDL_Rect Bounds = Shooter->GetComponent<TextureComponent>()->GetRectangle();
	int PosX = Bounds.x + Bounds.w / 2 - GetOwner()->GetComponent<TextureComponent>()->GetRectangle().w / 2;
	int PosY = Bounds.y + Bounds.h / 2 - GetOwner()->GetComponent<TextureComponent>()->GetRectangle().h / 2;

	m_RootComponentPtr->SetEntityPosition(PosX, PosY);
	
	int VelX = 0;
	int VelY = 0;
	int PositionShift = 10;
	EMovementDirection Direction = Shooter->GetComponent<MovementComponent>()->GetMovementDirection();

	switch (Direction) {
	case EMovementDirection::UP:
		PosY -= PositionShift;
		VelY = -VEL;
		break;
	case EMovementDirection::DOWN:
		PosY += PositionShift;
		VelY = VEL;
		break;
	case EMovementDirection::RIGHT:
		PosX += PositionShift;
		VelX = VEL;
		break;
	case EMovementDirection::LEFT:
		PosX -= PositionShift;
		VelX = -VEL;
		break;
	default:
		break;
	}

	m_MovementComponentPtr->SetVelocityX(VelX);
	m_MovementComponentPtr->SetVelocityY(VelY);
	GetOwner()->GetComponent<MovementComponent>()->SetMovementDirection(Direction);
	Shoot();
}

void ProjectileComponent::Shoot()
{
	Engine::Get()->AddToActiveScene(GetOwner());
	Engine::Get()->RegisterForCollision(m_CollisionComponentPtr);
	GetOwner()->GetComponent<TextureComponent>()->SetVisible(true);
	m_MovementComponentPtr->SetCanMove(true);
}

void ProjectileComponent::DestroySelf()
{
	f_EnemyHit = nullptr;
	f_WallDestroyed = nullptr;
		
	GetOwner()->GetComponent<TextureComponent>()->SetVisible(false);
	Engine::Get()->DeregisterFromCollision(m_CollisionComponentPtr);
	Engine::Get()->RemoveFromActiveScene(GetOwner());
	ObjectPool::Get()->PutbackProjectile(GetOwner());
}
