#include "EnemySpawnerComponent.h"

#include "../../Engine/Components/CollisionComponent.h"

#include "../../Engine/ResourceManager.h"

#include "Entity.h"
#include "Engine.h"
#include "ObjectPool.h"

#include "../GameState.h"

#include "EnemyActionsComponent.h"

#include <stdlib.h>   
#include <time.h> 
#include <SDL.h>

EnemySpawnerComponent::EnemySpawnerComponent(Entity* Owner)
	:EntityComponent(Owner)
	, m_WindowWidth(0)
	, m_WindowHeight(0)
	, m_ElapsedMidSpawn(0)
	, m_SpawnedCount(0)
	, m_SpawnGap(SPAWN_GAP_MIN)
	, m_NextSpawnSpotID(0)
	, m_NumObjectsInPoint({})
	, m_SpawnPointsColliders({})
{
	srand(time(NULL));
}

EnemySpawnerComponent::EnemySpawnerComponent()
	:EnemySpawnerComponent(nullptr)
{
}


void EnemySpawnerComponent::Initialize()
{

	int MaxWidth = 0, MaxHeight = 0;
	SDL_GetWindowSize(Engine::Get()->GetWindow(), &MaxWidth, &MaxHeight);
	m_WindowWidth = MaxWidth;
	m_WindowHeight = MaxHeight;

	PrepareSpawnPoints();
}



void EnemySpawnerComponent::Update(float DeltaTime)
{
	if (m_SpawnedCount >= GameState::NUMBER_ENEMIES)
		return;

	m_ElapsedMidSpawn += DeltaTime;
	if (m_ElapsedMidSpawn > m_SpawnGap) {
		GetNextSpawnSpotID();
		TryToSpawn();
	}
}

void EnemySpawnerComponent::PrepareSpawnPoints()
{
	const EntityComponent* ComponentPrototype = Engine::Get()->GetResourceManager()->GetComponentPrototypeByName("CollisionComponent");
	for (int i = 0; i < NUM_SPAWN_SPOTS; i++) {
		CollisionComponent* NewCollisionComponentPtr = (CollisionComponent*)ComponentPrototype->Clone();
		NewCollisionComponentPtr->Initialize();

		NewCollisionComponentPtr->SetSize(SPACE_GAP_UNIT, SPACE_GAP_UNIT);
		NewCollisionComponentPtr->SetPosition((i % 2) * m_WindowWidth + (1 - (i % 2) * 2) * (1 + (i % 2)) * SPACE_GAP_UNIT, (i / 2) * m_WindowHeight + (1 - (i / 2) * 2) * (1 + (i / 2)) * SPACE_GAP_UNIT);

		NewCollisionComponentPtr->SetCollisionResponse(ECollisionResponseType::OVERLAP);
		NewCollisionComponentPtr->SetCollisionMask(ECollisionMask::OBSTACLE);

		NewCollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::ENEMY);
		NewCollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER1);
		NewCollisionComponentPtr->AddCollisonToActiveMask(ECollisionMask::PLAYER2);
		NewCollisionComponentPtr->RemoveCollisonFromActiveMask(ECollisionMask::PROJECTILE);

		NewCollisionComponentPtr->BindOnOverlapBegin([this](CollisionComponent* Self, CollisionComponent* Other) {SpotEntered(Self, Other); });
		NewCollisionComponentPtr->BindOnOverlapEnd([this](CollisionComponent* Self, CollisionComponent* Other) {SpotExited(Self, Other); });

		NewCollisionComponentPtr->SetOwner(GetOwner());
		Engine::Get()->RegisterForCollision(NewCollisionComponentPtr);

		m_SpawnPointsColliders[i] = NewCollisionComponentPtr;
	}
}


void EnemySpawnerComponent::GetNextSpawnSpotID()
{
	m_NextSpawnSpotID = rand() % 4;
}

void EnemySpawnerComponent::GetNextSpawnGap()
{
	m_SpawnGap = SPAWN_GAP_MIN + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (SPAWN_GAP_MAX - SPAWN_GAP_MIN)));
}

void EnemySpawnerComponent::TryToSpawn() {

	if (m_NumObjectsInPoint[m_NextSpawnSpotID] != 0)
	{
		return;
	}
	m_ElapsedMidSpawn = 0;
	SpawnEnemy();
	GetNextSpawnGap();
}


void EnemySpawnerComponent::SpawnEnemy()
{
	Entity* Enemy = ObjectPool::Get()->GetEnemy();
	if (Enemy)
	{
		m_SpawnedCount++;
		SDL_Rect* SpawnBounds = m_SpawnPointsColliders[m_NextSpawnSpotID]->GetBounds();
		Enemy->GetComponent<EnemyActionsComponent>()->Setup(SpawnBounds->x, SpawnBounds->y);
	}
}

void EnemySpawnerComponent::SpotEntered(CollisionComponent* Self, CollisionComponent* Other)
{
	for (int i = 0; i < m_SpawnPointsColliders.size(); i++)
	{
		if (m_SpawnPointsColliders[i] == Self)
		{
			m_NumObjectsInPoint[i]++;
			break;
		}
	}
}

void EnemySpawnerComponent::SpotExited(CollisionComponent* Self, CollisionComponent* Other)
{
	for (int i = 0; i < m_SpawnPointsColliders.size(); i++)
	{
		if (m_SpawnPointsColliders[i] == Self)
		{
			m_NumObjectsInPoint[i]--;
			break;
		}
	}
}