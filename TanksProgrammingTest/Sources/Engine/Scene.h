#pragma once

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class ObjectPool;
class Entity;

class Scene
{

public:
	void LoadFromConfig(const nlohmann::json& Config);
	void Initialize();
	void Update(float DeltaTime);
	void Draw();
	void UnInitialize();

	void AddEntity(Entity* Entity);
	void RemoveEntity(Entity* Entity);
	void RegisterAllForCollision();

private:
	void LoadSceneFromLayout(const nlohmann::json& Content, const nlohmann::json& Legend);

	std::list<Entity*> m_Entities;
	std::string m_Name;


};