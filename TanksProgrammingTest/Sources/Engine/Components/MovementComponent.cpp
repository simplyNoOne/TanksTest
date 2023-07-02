#include "MovementComponent.h"

#include "CollisionComponent.h"
#include "Engine.h"
#include "Entity.h"
#include "TextureComponent.h"
#include "RootComponent.h"



MovementComponent::MovementComponent(Entity* Owner)
	: EntityComponent(Owner),
	m_CanMove(true)
	, m_VelocityX(0.f)
	, m_VelocityY(0.f)
{
}

MovementComponent::MovementComponent()
	: MovementComponent(nullptr)
{
}

void MovementComponent::LoadFromConfig(const nlohmann::json& Config)
{
	m_VelocityX = Config.value("VelocityX", 0);
	m_VelocityY = Config.value("VelocityY", 0);
	m_MovementDirection = static_cast<EMovementDirection>(Config.value("Direction", 0));
}

void MovementComponent::Initialize()
{
	m_PosX = 400;
	m_PosY = 230;
	UpdateSize();
	CollisionComponent* CollisionComponentPtr = GetOwner()->GetComponent<CollisionComponent>();
	if (CollisionComponentPtr)
	{
		CollisionComponentPtr->BindOnHit([this](CollisionComponent* Self, CollisionComponent* Other, ECollisionSide Side) {ObstacleHit(Self, Other, Side); });

	}
}

void MovementComponent::Update(float DeltaTime)
{
	if (!m_CanMove)
	{
		return;
	}
	Move(m_PosX + static_cast<int>(m_VelocityX * DeltaTime), m_PosY + static_cast<int>(m_VelocityY * DeltaTime));


}


void MovementComponent::Move(int NewX, int NewY)
{
	if (GetOwner()->GetComponent<RootComponent>())
	{
		GetOwner()->GetComponent<RootComponent>()->SetEntityPosition(NewX, NewY);
	}
}


void MovementComponent::UpdateSize()
{
	TextureComponent* TextureComponentPtr = GetOwner()->GetComponent<TextureComponent>();
	if (TextureComponentPtr)
	{
		m_Width = TextureComponentPtr->GetRectangle().w;
		m_Height = TextureComponentPtr->GetRectangle().h;
	}
}

void MovementComponent::ObstacleHit(CollisionComponent* Self, CollisionComponent* Obstacle, ECollisionSide Side)
{
	m_LastHitSide = Side;
	if (Obstacle->DirectionTowardSide(Side) != m_MovementDirection)
	{
		m_CanMove = true;
		return;
	}
	if (!m_CanMove)
	{
		return;
	}

	m_CanMove = false;
	m_VelocityX = 0;
	m_VelocityY = 0;

}