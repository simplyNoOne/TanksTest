#include "Collidable.h"

#include "Components/CollisionComponent.h"


Collidable::Collidable() :
	m_CollisionMask(ECollisionMask::OBSTACLE)
	, m_CollisionResponse(ECollisionResponseType::IGNORE)

{
	m_DirectionTowardSide[static_cast<int>(ECollisionSide::RIGHT)] = EMovementDirection::RIGHT;
	m_DirectionTowardSide[static_cast<int>(ECollisionSide::LEFT)] = EMovementDirection::LEFT;
	m_DirectionTowardSide[static_cast<int>(ECollisionSide::UP)] = EMovementDirection::UP;
	m_DirectionTowardSide[static_cast<int>(ECollisionSide::DOWN)] = EMovementDirection::DOWN;

	m_DirectionOppositeToSide[static_cast<int>(ECollisionSide::RIGHT)] = EMovementDirection::LEFT;
	m_DirectionOppositeToSide[static_cast<int>(ECollisionSide::LEFT)] = EMovementDirection::RIGHT;
	m_DirectionOppositeToSide[static_cast<int>(ECollisionSide::UP)] = EMovementDirection::DOWN;
	m_DirectionOppositeToSide[static_cast<int>(ECollisionSide::DOWN)] = EMovementDirection::UP;
}

void Collidable::Hit(CollisionComponent* OtherCollider)
{
}


void Collidable::AddCollisonToActiveMask(ECollisionMask Collision)
{
	m_ActiveCollisionMask.set(static_cast<int>(Collision), true);
}
void Collidable::RemoveCollisonFromActiveMask(ECollisionMask Collision)
{
	m_ActiveCollisionMask.set(static_cast<int>(Collision), false);
}

void Collidable::UpdateCollisions()
{
	for (auto it = m_ActiveOverlaps.begin(); it != m_ActiveOverlaps.end(); it++)
	{
		auto cached = m_CachedOverlaps.find(*it);
		if (cached != m_CachedOverlaps.end())
		{
			m_CachedOverlaps.erase(*it);
		}
		else
		{
			if (!f_OverlapBeginCalls.empty())
			{
				for (auto Call : f_OverlapBeginCalls)
				{
					Call(dynamic_cast<CollisionComponent*>(this), *it);
				}
			}
			if (!(*it)->f_OverlapBeginCalls.empty())
			{
				for (auto Call : (*it)->f_OverlapBeginCalls)
				{
					Call(*it, dynamic_cast<CollisionComponent*>(this));
				}
			}
		}
	}

	for (auto it = m_CachedOverlaps.begin(); it != m_CachedOverlaps.end(); it++)
	{
		if (!f_OverlapEndCalls.empty())
		{
			for (auto Call : f_OverlapEndCalls)
			{
				Call(dynamic_cast<CollisionComponent*>(this), *it);
			}
		}
		if (!(*it)->f_OverlapEndCalls.empty())
		{
			for (auto Call : (*it)->f_OverlapEndCalls)
			{
				Call(*it, dynamic_cast<CollisionComponent*>(this));
			}
		}
	}

	m_CachedOverlaps = m_ActiveOverlaps;
	m_ActiveOverlaps.clear();
}


bool Collidable::DoesCollideWith(ECollisionMask Collision) {
	return m_ActiveCollisionMask.test(static_cast<int>(Collision));
}

void Collidable::Collide(CollisionComponent* OtherCollider)
{
	if (m_CollisionResponse == ECollisionResponseType::BLOCK && OtherCollider->GetCollisionResponse() == ECollisionResponseType::BLOCK)
	{
		Hit(OtherCollider);
	}
	else
	{
		m_ActiveOverlaps.emplace(OtherCollider);
	}
}


