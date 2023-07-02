#include "../Engine/Components/CollisionComponent.h"
#include "../Engine/Components/RootComponent.h"
#include "../Engine/Components/TextureComponent.h"
#include "../Engine/Components/MovementComponent.h"

#include "../Engine/GameManager.h"

#include "Player/PlayerActionsComponent.h"
#include "Player/PlayerInputComponent.h"
#include "Player/PlayerMovementComponent.h"

#include "Enemy/EnemyActionsComponent.h"
#include "Enemy/EnemyMovementComponent.h"
#include "Enemy/EnemySpawnerComponent.h"

#include "Objects/ProjectileComponent.h"
#include "Objects/WallComponent.h"
#include "Objects/PlayerBaseComponent.h"

#include "Engine.h"
#include "ObjectPool.h"
#include "ResourceManager.h"
#include "Entity.h"
#include "GameState.h"
#include "MenuComponent.h"

int main(int argc, char* argv[])
{
	Engine::Get()->Initialize();

	ResourceManager* ResourceManagerPtr = Engine::Get()->GetResourceManager();

	ResourceManagerPtr->RegisterComponent("PlayerInputComponent", new PlayerInputComponent());
	ResourceManagerPtr->RegisterComponent("TextureComponent", new TextureComponent());
	ResourceManagerPtr->RegisterComponent("CollisionComponent", new CollisionComponent());
	ResourceManagerPtr->RegisterComponent("PlayerActionsComponent", new PlayerActionsComponent());
	ResourceManagerPtr->RegisterComponent("PlayerMovementComponent", new PlayerMovementComponent());
	ResourceManagerPtr->RegisterComponent("MovementComponent", new MovementComponent());
	ResourceManagerPtr->RegisterComponent("RootComponent", new RootComponent());
	ResourceManagerPtr->RegisterComponent("ProjectileComponent", new ProjectileComponent());
	ResourceManagerPtr->RegisterComponent("WallComponent", new WallComponent());
	ResourceManagerPtr->RegisterComponent("EnemySpawnerComponent", new EnemySpawnerComponent());
	ResourceManagerPtr->RegisterComponent("PlayerBaseComponent", new PlayerBaseComponent());
	ResourceManagerPtr->RegisterComponent("EnemyActionsComponent", new EnemyActionsComponent());
	ResourceManagerPtr->RegisterComponent("EnemyMovementComponent", new EnemyMovementComponent());
	ResourceManagerPtr->RegisterComponent("MenuComponent", new MenuComponent());

	GameManager::Get()->GoToStart();

	Engine::Get()->MainLoop();

	Engine::Get()->ShutDown();

	return 0;
}