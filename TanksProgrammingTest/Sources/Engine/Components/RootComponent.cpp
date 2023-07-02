#include "RootComponent.h"

#include "Entity.h"

RootComponent::RootComponent(Entity* Owner)
	:EntityComponent(Owner)
{
}

RootComponent::RootComponent()
	:RootComponent(nullptr)
{
}


void RootComponent::LoadFromConfig(const nlohmann::json& Config)
{
	m_X = Config.value("PositionX", 0);
	m_Y = Config.value("PositionY", 0);
}

void RootComponent::SetPosition(int PosX, int PosY)
{
	m_X = PosX;
	m_Y = PosY;
}

void RootComponent::SetEntityPosition(int PosX, int PosY)
{
	SetPosition(PosX, PosY);	//Position of the root must be set first, so that all the other components adjust to it
	for (auto Component : GetOwner()->GetAllComponents())
	{
		Component->SetPosition(PosX, PosY);
	}
}