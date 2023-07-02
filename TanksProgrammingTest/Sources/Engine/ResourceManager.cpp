#include "ResourceManager.h"

#include "Engine.h"
#include "EngineUtils.h"
#include "Entity.h"
#include "Components/EntityComponent.h"

#include <filesystem>
#include <fstream>
#include <regex>
#include <SDL.h>


ResourceManager::ResourceManager(const std::string& Path)
	: m_Path(Path)
{
}

void ResourceManager::LoadResources()
{
	LoadAllTexturesFromFolder("Images", m_Textures);
	LoadResourcesFromFolder("Entities", m_Entities);
	LoadResourcesFromFolder("Scenes", m_Scenes);
}

const nlohmann::json& ResourceManager::GetJsonConfig(const std::string& Name, enum ResourceType Type)
{
	switch (Type)
	{
	case ResourceType::Entity:
		return m_Entities[Name];
	case ResourceType::Scene:
		return m_Scenes[Name];
	}

	static nlohmann::json DefaultJsonConfig;
	return DefaultJsonConfig;
}

void ResourceManager::RegisterComponent(const std::string& Type, EntityComponent* Component)
{
	m_ComponentsPrototypes[Type] = Component;
}

const EntityComponent* ResourceManager::GetComponentPrototypeByName(const std::string& Name)
{
	std::map<std::string, EntityComponent*>::iterator ComponentPrototypeIt = m_ComponentsPrototypes.find(Name);
	if (ComponentPrototypeIt != m_ComponentsPrototypes.end())
	{
		return ComponentPrototypeIt->second;
	}

	return nullptr;
}

Entity* ResourceManager::CreateEntityFromDataTemplate(const std::string& Name)
{
	std::map<std::string, nlohmann::json>::iterator EntityDataTemplateIt = m_Entities.find(Name);
	if (EntityDataTemplateIt != m_Entities.end())
	{
		Entity* NewEntity = new Entity();
		NewEntity->LoadFromConfig(EntityDataTemplateIt->second);
		return NewEntity;
	}

	return nullptr;
}

void ResourceManager::LoadResourcesFromFolder(const std::string& Folder, std::map<std::string, nlohmann::json>& MapContainer)
{
	const std::string FolderPath = m_Path + "/" + Folder;
	for (const auto& Entry : std::filesystem::directory_iterator(FolderPath))
	{
		std::ifstream InFile(Entry.path());
		nlohmann::json JsonFile;
		InFile >> JsonFile;

		if (!JsonFile.is_null())
		{
			const std::wstring WStrFileName = Entry.path().stem();
			const std::string FileName = EngineUtils::WstringToString(WStrFileName);
			MapContainer.insert({ FileName, JsonFile });
		}

		InFile.close();
	}
}

void ResourceManager::LoadAllTexturesFromFolder(const std::string& Folder, std::map<std::string, SDL_Texture*>& MapContainer)
{
	const std::string FolderPath = m_Path + "/" + Folder;
	for (const auto& Entry : std::filesystem::directory_iterator(FolderPath))
	{
		std::string Name = std::filesystem::path(Entry.path()).filename().string();
		const std::string Path = Entry.path().string();
		SDL_Surface* Surface = IMG_Load(Path.c_str());
		SDL_Texture* Texture = SDL_CreateTextureFromSurface(Engine::Get()->GetRenderer(), Surface);
		SDL_FreeSurface(Surface);
		MapContainer.insert({ Name, Texture });
	}
}