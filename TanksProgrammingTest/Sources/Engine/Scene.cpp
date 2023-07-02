#include "Scene.h"

#include "Engine.h"
#include "Entity.h" 
#include "ResourceManager.h"
#include "Components/TextureComponent.h"
#include "ObjectPool.h"
#include "Components/CollisionComponent.h"
#include <fstream>
#include <Components/RootComponent.h>

void Scene::LoadFromConfig(const nlohmann::json& Config)
{
	m_Name = Config.value("Name", "");

	if (Config.find("Entities") != Config.end())
	{
		ResourceManager* ResourceManagerPtr = Engine::Get()->GetResourceManager();
		for (const auto& Item : Config["Entities"].items())
		{
			Entity* NewEntity = new Entity();

			const nlohmann::json& EntityConfig = Item.value();
			const std::string& TypeName = EntityConfig.value("Type", "");
			if (!TypeName.empty())
			{
				const nlohmann::json& EntityTemplateConfig = ResourceManagerPtr->GetJsonConfig(TypeName, ResourceType::Entity);
				NewEntity->LoadFromConfig(EntityTemplateConfig);
			}
			else
			{
				NewEntity->LoadFromConfig(Item.value());
			}

			AddEntity(NewEntity);
		}
	}

	nlohmann::json::const_iterator SecenLayoutIt = Config.find("SceneLayout");
	if (SecenLayoutIt != Config.end())
	{
		LoadSceneFromLayout((*SecenLayoutIt)["Content"], (*SecenLayoutIt)["Legend"]);
	}
}

void Scene::Initialize()
{
	for (Entity* Entity : m_Entities)
	{
		Entity->Initialize();
	}

}

void Scene::Update(float DeltaTime)
{
	for (Entity* Entity : m_Entities)
	{
		Entity->Update(DeltaTime);
	}
}

void Scene::Draw()
{
	for (Entity* Entity : m_Entities)
	{
		Entity->Draw();
	}
}

void Scene::UnInitialize()
{
	for (Entity* Entity : m_Entities)
	{
		Entity->UnInitialize();
	}
}

void Scene::AddEntity(Entity* Entity)
{
	m_Entities.push_back(Entity);
}

void Scene::RemoveEntity(Entity* Entity)
{
	auto RetIt = std::remove(m_Entities.begin(), m_Entities.end(), Entity);
}

void Scene::RegisterAllForCollision()
{
	for (auto EntityPtr : m_Entities) {
		CollisionComponent* CollisionComponentPtr = EntityPtr->GetComponent<CollisionComponent>();
		if (CollisionComponentPtr)
			Engine::Get()->RegisterForCollision(CollisionComponentPtr);
	}
}

void Scene::LoadSceneFromLayout(const nlohmann::json& Content, const nlohmann::json& Legend)
{
	int Row = 0;
	ResourceManager* ResourceManagerPtr = Engine::Get()->GetResourceManager();
	for (const auto& Item : Content.items())
	{
		int Column = 0;
		const std::string& Line = Item.value();
		for (char Character : Line)
		{
			if (Character != ' ')
			{
				const char Key[] = { Character, '\0' };
				nlohmann::json EntitySpecs = Legend[Key];

				Entity* NewEntity = ResourceManagerPtr->CreateEntityFromDataTemplate(EntitySpecs["Type"]);
				TextureComponent* TextureComponentPtr = NewEntity->GetComponent<TextureComponent>();
				RootComponent* RootComponentPtr = NewEntity->GetComponent<RootComponent>();
				int Width = EntitySpecs["Width"];
				int Height = EntitySpecs["Height"];
				if (RootComponentPtr)
				{
					RootComponentPtr->SetEntityPosition(Column * Width, Row * Height);
				}
				TextureComponentPtr->SetScale(Width, Height);

				AddEntity(NewEntity);
			}
			++Column;
		}
		++Row;
	}
}