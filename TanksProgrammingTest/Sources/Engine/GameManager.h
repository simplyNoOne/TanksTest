#pragma once

#include "../Engine/Components/EntityComponent.h"
#include "../Engine/EnumInfo.h"
#include "../Engine/Entity.h"
#include "../Game/GameState.h"
class Scene;

class GameManager
{

public:

	static GameManager* Get();

	Entity* GetTarget(EEnemyTarget Target);
	EGameMode GetGameMode() { return m_GameMode; }
	EPlayer GetPlayerID(Entity* Player);
	EBase GetBaseID(Entity* Base);

	void GoToStart();
	void PlayGame();
	void GoToResults();
	void QuitGame();


	EGameStage GetGameStage() { return m_GameStage; }
	EGameResult GetGameResult() { return m_GameResult; }

	Scene* GetNextScene() { return m_NextScene; }
	bool GetSwitchScene() { return m_SwitchScene; }
	void SceneSwitched() { m_SwitchScene = false; }
	void AddPlayerToScene(int PosX, int PosY);

	void NextAction(ESelectedOption Option);

	void WinGame();
	void LoseGame(Entity* Loser);


	void RegisterSelfAsPlayer(Entity* PlayerEntity);
	void RegisterSelfAsBase(Entity* BaseEntity);


private:
	GameManager();

	bool m_SwitchScene;

	Scene* m_NextScene;

	EGameMode m_GameMode;
	EGameResult m_GameResult;
	EGameStage m_GameStage;


	Entity* m_Player1;
	Entity* m_PlayerBase1;
	Entity* m_Player2;
	Entity* m_PlayerBase2;

};