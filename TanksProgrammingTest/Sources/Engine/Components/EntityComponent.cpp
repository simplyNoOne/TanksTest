#include "EntityComponent.h"

EntityComponent::EntityComponent(Entity* Owner)
	: m_Owner(Owner)
{
}

void EntityComponent::LoadFromConfig(const nlohmann::json& Config)
{
}

void EntityComponent::Initialize()
{
}

void EntityComponent::Update(float DeltaTime)
{
}

void EntityComponent::Draw()
{
}

void EntityComponent::UnInitialize()
{
}

void EntityComponent::SetPosition(int PosX, int PosY)
{
}