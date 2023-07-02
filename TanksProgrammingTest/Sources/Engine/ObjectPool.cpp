#include "ObjectPool.h"

#include "Components/CollisionComponent.h"
#include "Entity.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "../Game/GameState.h"



ObjectPool::ObjectPool()
{
}


ObjectPool* ObjectPool::Get()
{
	static ObjectPool s_Instance;
	return &s_Instance;
}
void ObjectPool::Initialize()
{
	ResourceManager* ResourceManagerPtr = Engine::Get()->GetResourceManager();
	for (int i = 0; i < GameState::NUMBER_ENEMIES; i++) {

		Entity* NewEnemy = ResourceManagerPtr->CreateEntityFromDataTemplate("Enemy");
		Engine::Get()->DeregisterFromCollision(NewEnemy->GetComponent<CollisionComponent>());
		for (auto Component : NewEnemy->GetAllComponents())
			Component->Initialize();
		m_Enemies.emplace(NewEnemy);
	}
	for (int i = 0; i < NUM_PROJECTILES; i++) {

		Entity* NewProjectile = ResourceManagerPtr->CreateEntityFromDataTemplate("Projectile");
		Engine::Get()->DeregisterFromCollision(NewProjectile->GetComponent<CollisionComponent>());
		for (auto Component : NewProjectile->GetAllComponents())
			Component->Initialize();
		m_Projectiles.push(NewProjectile);
	}
}
void ObjectPool::UnInitialize()
{
	while (!m_Projectiles.empty()) {
		m_Projectiles.front()->UnInitialize();
		m_Projectiles.pop();
	}
	while (!m_Enemies.empty()) {
		m_Enemies.front()->UnInitialize();
		m_Enemies.pop();
	}

}

Entity* ObjectPool::GetProjectile()
{
	if (m_Projectiles.empty())
	{
		return nullptr;
	}
	Entity* Return = m_Projectiles.front();
	m_Projectiles.pop();
	return Return;
}

Entity* ObjectPool::GetEnemy()
{
	if (m_Enemies.empty())
	{
		return nullptr;
	}
	Entity* Return = m_Enemies.front();
	m_Enemies.pop();
	return Return;

}

void ObjectPool::PutbackProjectile(Entity* Projectile)
{
	//making sure that when used again, projectile will still interact with everything
	Projectile->GetComponent<CollisionComponent>()->AddCollisonToActiveMask(ECollisionMask::ENEMY);
	Projectile->GetComponent<CollisionComponent>()->AddCollisonToActiveMask(ECollisionMask::PLAYER1);
	Projectile->GetComponent<CollisionComponent>()->AddCollisonToActiveMask(ECollisionMask::PLAYER2);
	Projectile->GetComponent<CollisionComponent>()->AddCollisonToActiveMask(ECollisionMask::BASE1);
	Projectile->GetComponent<CollisionComponent>()->AddCollisonToActiveMask(ECollisionMask::BASE2);
	m_Projectiles.emplace(Projectile);
}
