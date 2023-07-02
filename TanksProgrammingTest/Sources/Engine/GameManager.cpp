#include "GameManager.h"

#include "Entity.h"
#include "Engine.h"
#include "../Engine/ObjectPool.h"
#include "../Engine/Scene.h"
#include "../Engine/ResourceManager.h"
#include "../Engine/Components/RootComponent.h"
#include "../Game/MenuComponent.h"


GameManager::GameManager()
{
}

GameManager* GameManager::Get()
{
	static GameManager s_Instance;
	return &s_Instance;
}


Entity* GameManager::GetTarget(EEnemyTarget Target)
{
	switch (Target)
	{
	case EEnemyTarget::PLAYER1:
		return m_Player1;
		break;
	case EEnemyTarget::PLAYER2:
		return m_Player2;
		break;
	case EEnemyTarget::BASE1:
		return m_PlayerBase1;
		break;
	case EEnemyTarget::BASE2:
		return m_PlayerBase2;
		break;
	}
	return nullptr;
}

EPlayer GameManager::GetPlayerID(Entity* Player)
{
	if (Player == m_Player1)
	{
		return EPlayer::PLAYER1;
	}
	else if (Player == m_Player2)
	{
		return EPlayer::PLAYER2;
	}
	else
	{
		return EPlayer::NON_PLAYER;
	}
}

EBase GameManager::GetBaseID(Entity* Base)
{
	if (Base == m_PlayerBase1)
	{
		return EBase::BASE1;
	}
	else if (Base == m_PlayerBase2)
	{
		return EBase::BASE2;
	}
	else
	{
		return EBase::NON_BASE;
	}
}


void GameManager::GoToStart()
{
	m_GameMode = EGameMode::SINGLEPLAYER;
	m_GameStage = EGameStage::STARTMENU;
	m_NextScene = Engine::Get()->CreateSceneFromTemplate("StartScene");
	m_SwitchScene = true;

}

void GameManager::PlayGame()
{
	ObjectPool::Get()->Initialize();

	m_GameStage = EGameStage::GAME;

	if (m_GameMode == EGameMode::SINGLEPLAYER)
	{
		m_NextScene = Engine::Get()->CreateSceneFromTemplate("MainScene1");
		AddPlayerToScene(GameState::SINGLEPLAYER_X, GameState::SINGLEPLAYER_Y);
	}
	else
	{
		m_NextScene = Engine::Get()->CreateSceneFromTemplate("MainScene2");
		AddPlayerToScene(GameState::PLAYER1_X, GameState::PLAYER1_Y);
		AddPlayerToScene(GameState::PLAYER2_X, GameState::PLAYER2_Y);
	}
	m_SwitchScene = true;
}


void GameManager::GoToResults()
{
	m_Player1 = nullptr;
	m_Player2 = nullptr;
	m_PlayerBase1 = nullptr;
	m_PlayerBase2 = nullptr;

	ObjectPool::Get()->UnInitialize();

	m_GameStage = EGameStage::RESULTS;
	m_NextScene = Engine::Get()->CreateSceneFromTemplate("ResultsScene");
	m_SwitchScene = true;

}

void GameManager::QuitGame()
{
	m_NextScene = nullptr;
	m_SwitchScene = true;
}

void GameManager::AddPlayerToScene(int PosX, int PosY)
{
	Entity* NewPlayer = Engine::Get()->GetResourceManager()->CreateEntityFromDataTemplate("Player");
	NewPlayer->GetComponent<RootComponent>()->SetEntityPosition(PosX, PosY);
	m_NextScene->AddEntity(NewPlayer);
}



void GameManager::NextAction(ESelectedOption Option)
{
	if (m_GameStage == EGameStage::STARTMENU)
	{
		if (Option == ESelectedOption::OPTION1)
		{
			m_GameMode = EGameMode::SINGLEPLAYER;
			GameState::NUMBER_ENEMIES = GameState::NUMBER_ENEMIES_SINGLEPLAYER;
		}
		else
		{
			m_GameMode = EGameMode::MULTIPLAYER;
			GameState::NUMBER_ENEMIES = GameState::NUMBER_ENEMIES_MULTIPLAYER;
		}
		PlayGame();
	}
	else if (m_GameStage == EGameStage::RESULTS)
	{
		if (Option == ESelectedOption::OPTION1)
		{
			GoToStart();
		}
		else
		{
			QuitGame();
		}
	}
}


void GameManager::WinGame()
{
	m_GameResult = EGameResult::PLAYER1_WON;
	GoToResults();

}

void GameManager::LoseGame(Entity* Loser)
{
	m_GameResult = (Loser == m_Player1 || Loser == m_PlayerBase1) ? EGameResult::PLAYER1_LOST : EGameResult::PLAYER1_WON;
	GoToResults();
}


void GameManager::RegisterSelfAsPlayer(Entity* PlayerEntity)
{
	if (!m_Player1)
	{
		m_Player1 = PlayerEntity;
	}
	else if (!m_Player2)
	{
		m_Player2 = PlayerEntity;
	}
}

void GameManager::RegisterSelfAsBase(Entity* BaseEntity)
{
	if (!m_PlayerBase1)
	{
		m_PlayerBase1 = BaseEntity;
	}
	else if (!m_PlayerBase2)
	{
		m_PlayerBase2 = BaseEntity;
	}
}

