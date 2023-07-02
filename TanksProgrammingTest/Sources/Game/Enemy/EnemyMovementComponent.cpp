#include "EnemyMovementComponent.h"


#include "../../Engine/Components/CollisionComponent.h"
#include "../../Engine/Components/TextureComponent.h"
#include "../../Engine/Components/RootComponent.h"

#include "../../Engine/GameManager.h"

#include "../GameState.h"

#include "EnemyActionsComponent.h"

#include "Engine.h"
#include "Entity.h"


EnemyMovementComponent::EnemyMovementComponent(Entity* Owner)
	: MovementComponent(Owner)
	, m_TextureComponentPtr(nullptr)
	, m_EnemyActionsComponentPtr(nullptr)
	, m_RootComponentPtr(nullptr)
	, m_MidTurnElapsed(0.f)
{
}

EnemyMovementComponent::EnemyMovementComponent()
	: EnemyMovementComponent(nullptr)
{
}

void EnemyMovementComponent::Initialize()
{
	MovementComponent::Initialize();

	m_EnemyActionsComponentPtr = GetOwner()->GetComponent<EnemyActionsComponent>();
	m_TextureComponentPtr = GetOwner()->GetComponent<TextureComponent>();
	m_RootComponentPtr = GetOwner()->GetComponent<RootComponent>();
	GetOwner()->GetComponent<CollisionComponent>()->SetSize(30, 30);
	m_CanMove = false;
	m_VelocityY = VEL;

}

void EnemyMovementComponent::Update(float DeltaTime)
{
	m_MidTurnElapsed += DeltaTime;
	MovementComponent::Update(DeltaTime);
}



void EnemyMovementComponent::AreasEndOverlap(CollisionComponent* Self, CollisionComponent* Other)
{
	int AreaID = m_EnemyActionsComponentPtr->GetCollisionAreaID(Self);
	if (m_EnemyActionsComponentPtr->GetNumOverlapsInAreaID(AreaID) == 0)
	{
		EMovementDirection Direction = GetNewOptimalDirection();
		if (Direction == static_cast<EMovementDirection>(AreaID))
		{
			TurnInDirection(Direction);
		}
	}
}

void EnemyMovementComponent::Turn()
{
	if (!m_EnemyActionsComponentPtr->GetWaitingToTurn())
	{
		TurnInDirection(GetNewOptimalDirection());
	}
}

void EnemyMovementComponent::ObjectHit(CollisionComponent* Self, CollisionComponent* Other, ECollisionSide Side)
{
	if (Self->DirectionTowardSide(Side) == m_MovementDirection)
	{
		Turn();
	}
}


EMovementDirection EnemyMovementComponent::GetNewOptimalDirection()
{
	EMovementDirection OptimalDirection = CalculateDirectionToTarget();
	if (OptimalDirection == m_MovementDirection && !m_CanMove)
	{
		OptimalDirection = GetRandomDirection();
	}
	if (m_EnemyActionsComponentPtr->GetNumOverlapsInAreaID(static_cast<int>(OptimalDirection)) != 0)
	{
		m_EnemyActionsComponentPtr->DelayTurnAndShoot(OptimalDirection);
	}
	return OptimalDirection;
}

EMovementDirection EnemyMovementComponent::GetRandomDirection()
{
	EMovementDirection NewDir;
	do {
		NewDir = (EMovementDirection)(rand() % 4);
	} while (NewDir == m_MovementDirection);
	return NewDir;
}

EMovementDirection EnemyMovementComponent::CalculateDirectionToTarget()
{
	EMovementDirection DesiredDirection = m_MovementDirection;
	if (m_EnemyActionsComponentPtr->GetDestroyedTarget())
	{
		return GetRandomDirection();
	}
	Entity* Target = GameManager::Get()->GetTarget(m_EnemyActionsComponentPtr->GetTarget());
	if (!Target || !Target->GetComponent<RootComponent>()) {
		return GetRandomDirection();
	}
	int DiffX = Target->GetComponent<RootComponent>()->GetX() - m_RootComponentPtr->GetX();
	int DiffY = Target->GetComponent<RootComponent>()->GetY() - m_RootComponentPtr->GetY();

	EMovementDirection DirectionX = (DiffX > 0) ? EMovementDirection::RIGHT : EMovementDirection::LEFT;
	EMovementDirection DirectionY = (DiffY > 0) ? EMovementDirection::DOWN : EMovementDirection::UP;

	DesiredDirection = (abs(DiffX) > abs(DiffY)) ? DirectionX : DirectionY;

	return DesiredDirection;
}

void EnemyMovementComponent::TurnInDirection(EMovementDirection Direction) {
	if (m_MovementDirection == Direction || m_MidTurnElapsed < TURN_COOLDOWN)
	{
		return;
	}
	m_MovementDirection = Direction;
	switch ((EMovementDirection)Direction) {
	case EMovementDirection::UP:
		m_VelocityX = 0;
		m_VelocityY = -VEL;
		m_TextureComponentPtr->SetTextureFromAssetName("BlueTankFront.png");
		m_EnemyActionsComponentPtr->SetCollisionsRelativePositions(8, 3);
		break;

	case EMovementDirection::DOWN:
		m_VelocityX = 0;
		m_VelocityY = VEL;
		m_TextureComponentPtr->SetTextureFromAssetName("BlueTankDown.png");
		m_EnemyActionsComponentPtr->SetCollisionsRelativePositions(8, 10);
		break;

	case EMovementDirection::RIGHT:
		m_VelocityX = VEL;
		m_VelocityY = 0;
		m_TextureComponentPtr->SetTextureFromAssetName("BlueTankRight.png");
		m_EnemyActionsComponentPtr->SetCollisionsRelativePositions(11, 5);
		break;

	case EMovementDirection::LEFT:
		m_VelocityX = -VEL;
		m_VelocityY = 0;
		m_TextureComponentPtr->SetTextureFromAssetName("BlueTankLeft.png");
		m_EnemyActionsComponentPtr->SetCollisionsRelativePositions(3, 5);
		break;

	}

	Move();
	m_CanMove = true;
	m_MidTurnElapsed = 0.f;
}