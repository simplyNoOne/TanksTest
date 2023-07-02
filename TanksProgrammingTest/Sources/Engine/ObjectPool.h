#pragma once

#include "ObjectPool.h"
#include <queue>


class Entity;

class ObjectPool
{

public:

	static ObjectPool* Get();

	void Initialize();
	void UnInitialize();

	Entity* GetProjectile();
	Entity* GetEnemy();

	void PutbackProjectile(Entity* Projectile);

private:

	ObjectPool();
	std::queue<Entity*>m_Enemies;
	std::queue<Entity*>m_Projectiles;

	const int NUM_PROJECTILES = 40;

};