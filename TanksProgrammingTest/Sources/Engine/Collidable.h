#pragma once

#include "Components/EntityComponent.h"
#include "EnumInfo.h"
#include <unordered_set>
#include <bitset>
#include <array>

class TextureComponent;
class CollisionComponent;


typedef std::function<void(CollisionComponent*, CollisionComponent*)> OverlapCallback;
typedef std::function<void(CollisionComponent*, CollisionComponent*, ECollisionSide)> CollisionCallback;


constexpr int NUM_SIDES = 4;
class Collidable {
public:

	Collidable();


	virtual void Hit(CollisionComponent* OtherCollider);

	void SetCollisionMask(ECollisionMask Mask) { m_CollisionMask = Mask; }
	void SetCollisionResponse(ECollisionResponseType Response) { m_CollisionResponse = Response; }

	ECollisionMask GetCollisionMask() { return m_CollisionMask; }
	ECollisionResponseType GetCollisionResponse() { return m_CollisionResponse; }


	void BindOnOverlapBegin(OverlapCallback Call) { f_OverlapBeginCalls.push_back(Call); }
	void BindOnOverlapEnd(OverlapCallback Call) { f_OverlapEndCalls.push_back(Call); }
	void BindOnHit(CollisionCallback Call) { f_HitCalls.push_back(Call); }


	void AddCollisonToActiveMask(ECollisionMask Collision);
	void RemoveCollisonFromActiveMask(ECollisionMask Collision);

	EMovementDirection DirectionTowardSide(ECollisionSide Side) { return m_DirectionTowardSide[(int)Side]; }
	EMovementDirection DirectionOppositeToSide(ECollisionSide Side) { return m_DirectionOppositeToSide[(int)Side]; }

	bool DoesCollideWith(ECollisionMask Collision);
	void Collide(CollisionComponent* OtherCollider);
	void UpdateCollisions();

protected:



	ECollisionResponseType m_CollisionResponse;
	ECollisionMask m_CollisionMask;
	std::bitset<8> m_ActiveCollisionMask;

	std::unordered_set<CollisionComponent*> m_ActiveOverlaps;
	std::unordered_set<CollisionComponent*> m_CachedOverlaps;

	std::vector<OverlapCallback> f_OverlapBeginCalls;
	std::vector<OverlapCallback> f_OverlapEndCalls;
	std::vector<CollisionCallback> f_HitCalls;

	std::array<EMovementDirection, NUM_SIDES> m_DirectionTowardSide;
	std::array<EMovementDirection, NUM_SIDES> m_DirectionOppositeToSide;

};
