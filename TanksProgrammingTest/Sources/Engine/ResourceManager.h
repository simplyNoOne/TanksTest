#pragma once

#include <string>
#include <map>
#include <nlohmann/json.hpp>

enum class ResourceType : unsigned int
{
	Entity,
	Scene
};

class Entity;
class EntityComponent;
class SDL_Texture;

class ResourceManager
{

public:
	ResourceManager() = delete;
	ResourceManager(const std::string& Path);

	void LoadResources();
	const nlohmann::json& GetJsonConfig(const std::string& Name, enum ResourceType Type);
	void RegisterComponent(const std::string& Type, EntityComponent* Component);
	const EntityComponent* GetComponentPrototypeByName(const std::string& Name);
	Entity* CreateEntityFromDataTemplate(const std::string& Name);
	SDL_Texture* GetTextureByName(std::string Name) { return m_Textures.at(Name); }

private:
	void LoadResourcesFromFolder(const std::string& Folder, std::map<std::string, nlohmann::json>& MapContainer);
	void LoadAllTexturesFromFolder(const std::string& Folder, std::map<std::string, SDL_Texture*>& MapContainer);

	std::string m_Path;
	std::map<std::string, nlohmann::json> m_Entities;
	std::map<std::string, nlohmann::json> m_Scenes;

	std::map<std::string, EntityComponent*> m_ComponentsPrototypes;
	std::map<std::string, SDL_Texture*> m_Textures;
};