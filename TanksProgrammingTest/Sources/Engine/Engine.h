#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <vector>
#include <string>

class Scene;
class ResourceManager;
class CollisionComponent;


class Entity;
class Engine
{

public:
	static Engine* Get();

	void Initialize();
	void MainLoop();
	void Draw();
	void CheckCollisions();
	void ShutDown();

	void SetActiveScene(Scene* Scene) { m_ActiveScene = Scene; }

	SDL_Renderer* GetRenderer() { return m_Renderer; }
	SDL_Window* GetWindow() { return m_Window; }
	ResourceManager* GetResourceManager() { return m_ResourceManager; }
	const std::vector<SDL_Event>& GetEvents() { return m_Events; }

	void CreateActiveSceneFromTemplate(const std::string& Name);
	Scene* CreateSceneFromTemplate(const std::string& Name);

	void AddToActiveScene(Entity* Entity);
	void RemoveFromActiveScene(Entity* Entity);

	void CloseActiveScene();
	void CloseGame() { m_Close = true; }

	void RegisterForCollision(CollisionComponent* Collider);
	void DeregisterFromCollision(CollisionComponent* Collider);



private:
	Engine();

	bool m_Close;

	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	Scene* m_ActiveScene;
	ResourceManager* m_ResourceManager;

	unsigned int FramesPerSecond;
	unsigned int TimePerFrameInMs;
	float TimePerFramInSceonds;

	std::vector<SDL_Event> m_Events;
	std::vector<CollisionComponent*> m_Colliders;
};