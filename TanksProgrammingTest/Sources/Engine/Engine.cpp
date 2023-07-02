#include "Engine.h"

#include "ResourceManager.h"
#include "Scene.h"
#include "Components/CollisionComponent.h"
#include <SDL.h>
#include <stdio.h>
#include "ObjectPool.h"
#include "GameManager.h"

Engine::Engine()
	: m_Window(nullptr)
	, m_Renderer(nullptr)
	, m_ActiveScene(nullptr)
	, m_ResourceManager(nullptr)
	, FramesPerSecond(60)
	, TimePerFrameInMs(1000 / FramesPerSecond)
	, TimePerFramInSceonds(TimePerFrameInMs / 1000.f)
	, m_Close(false)
{
}

Engine* Engine::Get()
{
	static Engine s_Instance;
	return &s_Instance;
}

void Engine::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	m_Window = SDL_CreateWindow("TanksProgrammingTest",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		870, 560, 0);

	Uint32 renderFlags = SDL_RENDERER_ACCELERATED;

	m_Renderer = SDL_CreateRenderer(m_Window, -1, renderFlags);

	m_ResourceManager = new ResourceManager("Resources");
	m_ResourceManager->LoadResources();

	if (m_ActiveScene != nullptr)
	{
		m_ActiveScene->Initialize();
	}
}

void Engine::MainLoop()
{

	unsigned int CurrentTime = SDL_GetTicks();
	unsigned int LastTime = CurrentTime;

	while (!m_Close)
	{
		if (GameManager::Get()->GetSwitchScene())
		{
			CloseActiveScene();
			Scene* NewScene = GameManager::Get()->GetNextScene();
			if (NewScene)
			{
				m_ActiveScene = NewScene;
				m_ActiveScene->Initialize();
				m_ActiveScene->RegisterAllForCollision();
			}
			else
			{
				CloseGame();
			}
			GameManager::Get()->SceneSwitched();

		}
		CurrentTime = SDL_GetTicks();
		while (!m_Close && CurrentTime > LastTime + TimePerFrameInMs)
		{
			m_Events.clear();
			SDL_Event Event;
			while (SDL_PollEvent(&Event))
			{
				if (Event.type == SDL_QUIT || (Event.type == SDL_KEYDOWN && Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
				{
					m_Close = true;
					break;
				}
				else
				{
					if (Event.key.repeat == 0)//no need to register multiple events when key is pressed once and not released
					{
						m_Events.push_back(Event);
					}
				}
			}

			if (m_ActiveScene != nullptr)
			{
				m_ActiveScene->Update(TimePerFramInSceonds);

			}
			LastTime += TimePerFrameInMs;

		}
		CheckCollisions();
		Draw();
	}
}

void Engine::Draw()
{
	SDL_RenderClear(m_Renderer);

	if (m_ActiveScene != nullptr)
	{
		m_ActiveScene->Draw();
	}

	SDL_RenderPresent(m_Renderer);
}

void Engine::CheckCollisions()
{
	if (m_Colliders.size() > 1)
	{
		for (unsigned int i = 0; i < m_Colliders.size() - 1; i++)
		{
			CollisionComponent* A = m_Colliders[i];
			if (A->GetCollisionResponse() == ECollisionResponseType::IGNORE)
			{
				continue;
			}

			for (unsigned int j = i + 1; j < m_Colliders.size(); j++)
			{
				CollisionComponent* B = m_Colliders[j];
				if (A->DoesCollideWith(B->GetCollisionMask()) && B->GetCollisionResponse() != ECollisionResponseType::IGNORE)
				{
					if (!(B->DoesCollideWith(A->GetCollisionMask())))
					{
						continue;
					}
					if (SDL_HasIntersection(A->GetBounds(), B->GetBounds()))
					{
						A->Collide(B);
						B->Collide(A);
					}
				}
			}
			A->UpdateCollisions();
		}
	}
}


void Engine::ShutDown()
{
	if (m_ActiveScene != nullptr)
	{
		m_ActiveScene->UnInitialize();
	}

	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}



void Engine::CreateActiveSceneFromTemplate(const std::string& Name)
{
	if (m_ResourceManager != nullptr)
	{
		const nlohmann::json& SceneConfig = m_ResourceManager->GetJsonConfig(Name, ResourceType::Scene);
		Scene* SceneFromTemplate = new Scene();
		SceneFromTemplate->LoadFromConfig(SceneConfig);
		SceneFromTemplate->Initialize();

		m_ActiveScene = SceneFromTemplate;
	}
}

Scene* Engine::CreateSceneFromTemplate(const std::string& Name)
{
	if (m_ResourceManager != nullptr)
	{
		const nlohmann::json& SceneConfig = m_ResourceManager->GetJsonConfig(Name, ResourceType::Scene);
		Scene* SceneFromTemplate = new Scene();
		SceneFromTemplate->LoadFromConfig(SceneConfig);
		return SceneFromTemplate;
	}
	return nullptr;
}

void Engine::AddToActiveScene(Entity* Entity)
{
	m_ActiveScene->AddEntity(Entity);
}

void Engine::RemoveFromActiveScene(Entity* Entity)
{
	m_ActiveScene->RemoveEntity(Entity);
}

void Engine::CloseActiveScene()
{
	if (m_ActiveScene)
	{
		m_ActiveScene->UnInitialize();
	}
	m_Colliders.clear();
}

void Engine::RegisterForCollision(CollisionComponent* Collider)
{
	m_Colliders.push_back(Collider);
}

void Engine::DeregisterFromCollision(CollisionComponent* Collider)
{
	for (auto it = m_Colliders.begin(); it < m_Colliders.end(); it++)
	{
		if (*it == Collider)
		{
			m_Colliders.erase(it);
			break;
		}
	}
}