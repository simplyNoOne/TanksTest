#pragma once

#include "../../Engine/Components/EntityComponent.h"
#include "../Engine/EnumInfo.h"
#include <vector>

class CollisionComponent;
class EnemySpawnerComponent : public EntityComponent
{
public:
	EnemySpawnerComponent(Entity* Owner);
	EnemySpawnerComponent();

	virtual EntityComponent* Clone() const override { return new EnemySpawnerComponent(*this); }

	virtual void Initialize() override;
	virtual void Update(float DeltaTime) override;

private:

	void PrepareSpawnPoints();

	void GetNextSpawnSpotID();
	void GetNextSpawnGap();

	void TryToSpawn();
	void SpawnEnemy();

	void SpotEntered(CollisionComponent* Self, CollisionComponent* Other);
	void SpotExited(CollisionComponent* Self, CollisionComponent* Other);




private:

	static const int NUM_SPAWN_SPOTS = 4;
	const int SPAWN_GAP_MIN = 5;
	const int SPAWN_GAP_MAX = 12;
	const int SPACE_GAP_UNIT = 35;

	int m_WindowWidth;
	int m_WindowHeight;

	float m_SpawnGap;
	float m_ElapsedMidSpawn;

	int m_SpawnedCount;
	int m_NextSpawnSpotID;


	std::array<int, NUM_SPAWN_SPOTS> m_NumObjectsInPoint;
	std::array<CollisionComponent*, NUM_SPAWN_SPOTS> m_SpawnPointsColliders;

};