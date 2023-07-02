#include "Entity.h"

#include "Engine.h"
#include "Components/EntityComponent.h"
#include "ResourceManager.h"
#include "Components/RootComponent.h";

void Entity::LoadFromConfig(const nlohmann::json& Config)
{
	m_Name = Config.value("Name", "");

	ResourceManager* ResourceManagerPtr = Engine::Get()->GetResourceManager();

	if (Config.find("Components") != Config.end())
	{
		for (const auto& ComponentItem : Config["Components"].items())
		{
			const nlohmann::json& ComponentConfig = ComponentItem.value();
			const std::string& Type = ComponentConfig["Type"];
			const EntityComponent* ComponentPrototype = ResourceManagerPtr->GetComponentPrototypeByName(Type);
			EntityComponent* NewComponent = ComponentPrototype->Clone();
			NewComponent->SetOwner(this);
			NewComponent->LoadFromConfig(ComponentConfig);
			AddComponent(NewComponent);
		}
	}
}

void Entity::Initialize()
{

	for (EntityComponent* Component : m_Components)
	{
		Component->Initialize();
	}
	for (EntityComponent* Component : m_Components)
	{
		if (!GetComponent<RootComponent>())
			continue;
		Component->SetPosition(GetComponent<RootComponent>()->GetX(), GetComponent<RootComponent>()->GetY());
	}
}

void Entity::Update(float DeltaTime)
{
	for (EntityComponent* Component : m_Components)
	{
		Component->Update(DeltaTime);
	}
}

void Entity::Draw()
{
	for (EntityComponent* Component : m_Components)
	{
		Component->Draw();
	}
}

void Entity::UnInitialize()
{
	for (EntityComponent* Component : m_Components)
	{
		Component->UnInitialize();
	}
}

void Entity::AddComponent(EntityComponent* Component)
{
	m_Components.push_back(Component);
}

void Entity::RemoveComponent(EntityComponent* Component)
{
	auto RetIt = std::remove(m_Components.begin(), m_Components.end(), Component);
}