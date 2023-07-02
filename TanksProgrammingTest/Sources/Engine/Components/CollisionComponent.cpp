#include "CollisionComponent.h"

#include "Entity.h"
#include "MovementComponent.h"
#include "TextureComponent.h"
#include "../Engine/Engine.h"
#include "../ResourceManager.h"


CollisionComponent::CollisionComponent(Entity* Owner)
	: EntityComponent(Owner),
	m_Bounds{ 0,0,0,0 }
{
}

CollisionComponent::CollisionComponent()
	:CollisionComponent(nullptr)
{
}


void CollisionComponent::Initialize()
{
}

void CollisionComponent::UnInitialize()
{
	Engine::Get()->DeregisterFromCollision(this);
}

void CollisionComponent::LoadFromConfig(const nlohmann::json& Config)
{
	m_Bounds.w = Config.value("Width", 10);
	m_Bounds.h = Config.value("Height", 10);

	m_RelativeX = Config.value("RelativeX", 0);
	m_RelativeY = Config.value("RelativeY", 0);

	SetCollisionResponse(static_cast<ECollisionResponseType>(Config.value("Response", 0)));
	SetCollisionMask(static_cast<ECollisionMask>(Config.value("Mask", 3)));
}


void CollisionComponent::SetPosition(int PosX, int PosY)
{
	m_Bounds.x = PosX + m_RelativeX;
	m_Bounds.y = PosY + m_RelativeY;
}

void CollisionComponent::SetRelative(int X, int Y)
{
	m_RelativeX = X;
	m_RelativeY = Y;
}

void CollisionComponent::Hit(CollisionComponent* OtherCollider)
{

	MovementComponent* MovementComponentPtr = GetOwner()->GetComponent<MovementComponent>();
	MovementComponent* OtherMovementComponentPtr = OtherCollider->GetOwner()->GetComponent<MovementComponent>();

	EMovementDirection MovementDir = MovementComponentPtr ? MovementComponentPtr->GetMovementDirection() : EMovementDirection::STATIC;
	EMovementDirection OtherMovementDir = OtherMovementComponentPtr ? OtherMovementComponentPtr->GetMovementDirection() : EMovementDirection::STATIC;

	ECollisionSide CollisionSide = CheckCollisionSide(OtherCollider);


	if (MovementDir == EMovementDirection::STATIC && OtherMovementDir == EMovementDirection::STATIC)
	{
		return;			//Two stationary bodies can't actively hitting each other
	}

	else {
		bool CollisionOccured = (DirectionTowardSide(CollisionSide) == MovementDir || DirectionOppositeToSide(CollisionSide) == OtherMovementDir);
		if (CollisionOccured)
		{
			if (!f_HitCalls.empty())
			{
				for (auto Call : f_HitCalls)
				{
					Call(dynamic_cast<CollisionComponent*>(this), OtherCollider, CollisionSide);
				}
			}
		}
	}

}

ECollisionSide CollisionComponent::CheckCollisionSide(CollisionComponent* OtherCollider)
{
	const SDL_Rect* OtherBounds = OtherCollider->GetBounds();

	int overlapY = std::min(m_Bounds.y + m_Bounds.h, OtherBounds->y + OtherBounds->h) - std::max(m_Bounds.y, OtherBounds->y);
	int overlapX = std::min(m_Bounds.x + m_Bounds.w, OtherBounds->x + OtherBounds->w) - std::max(m_Bounds.x, OtherBounds->x);

	if (overlapY > overlapX)
	{
		if (m_Bounds.x < OtherBounds->x)
		{
			return ECollisionSide::RIGHT;
		}
		else
		{
			return ECollisionSide::LEFT;
		}
	}
	else
	{
		if (m_Bounds.y < OtherBounds->y)
		{
			return ECollisionSide::DOWN;
		}

		else
		{
			return ECollisionSide::UP;
		}
	}
}


