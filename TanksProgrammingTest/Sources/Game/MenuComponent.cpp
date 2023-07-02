#include "MenuComponent.h"


#include "../Engine/Components/TextureComponent.h"
#include "../Engine/GameManager.h"
#include "../Engine/ResourceManager.h"

#include "Player/PlayerInputComponent.h"

#include "Entity.h"
#include "Engine.h"
#include "GameState.h"
#include <SDL.h>

MenuComponent::MenuComponent(Entity* Owner)
	:EntityComponent(Owner)
	, m_Option(ESelectedOption::OPTION1)
	, m_PosX(POS_LEFT)
	, m_ResultsMessage(nullptr)
	, m_MessageBounds({ 0, 0, 0, 0 })

{
}

MenuComponent::MenuComponent()
	:MenuComponent(nullptr)
{
}


void MenuComponent::Initialize()
{
	GetOwner()->GetComponent<TextureComponent>()->SetPosition(m_PosX, POSY);
	PlayerInputComponent* PlayerInputComponentPtr = GetOwner()->GetComponent<PlayerInputComponent>();
	if (PlayerInputComponentPtr) {
		PlayerInputComponentPtr->BindAxisRight1([this](int value) { SwitchChoice(value); });
		PlayerInputComponentPtr->BindAxisRight2([this](int value) { SwitchChoice(value); });
		PlayerInputComponentPtr->BindActionConfirm([this]() { Confirm(); });

		if (GameManager::Get()->GetGameStage() == EGameStage::RESULTS)
		{
			if (GameManager::Get()->GetGameMode() == EGameMode::SINGLEPLAYER)
			{
				if (GameManager::Get()->GetGameResult() == EGameResult::PLAYER1_WON)
				{
					m_ResultsMessage = Engine::Get()->GetResourceManager()->GetTextureByName("WinSingleplayer.png");
				}
				else
				{
					m_ResultsMessage = Engine::Get()->GetResourceManager()->GetTextureByName("LoseSingleplayer.png");
				}

			}
			else
			{
				if (GameManager::Get()->GetGameResult() == EGameResult::PLAYER1_WON)
				{
					m_ResultsMessage = Engine::Get()->GetResourceManager()->GetTextureByName("Player1.png");
				}
				else
				{
					m_ResultsMessage = Engine::Get()->GetResourceManager()->GetTextureByName("Player2.png");
				}
			}
		}
		else if (GameManager::Get()->GetGameStage() == EGameStage::STARTMENU)
		{
			m_ResultsMessage = Engine::Get()->GetResourceManager()->GetTextureByName("Title.png");
		}

	}
	m_MessageBounds.x = 320;
	m_MessageBounds.y = 100;
	m_MessageBounds.w = 200;
	m_MessageBounds.h = 80;
}

void MenuComponent::Draw()
{
	SDL_RenderCopy(Engine::Get()->GetRenderer(), m_ResultsMessage, nullptr, &m_MessageBounds);
}


void MenuComponent::SwitchChoice(int Value)
{
	if (Value == 0)
		return;
	if (m_Option == ESelectedOption::OPTION1)
	{
		m_Option = ESelectedOption::OPTION2;
		m_PosX = POS_RIGHT;
	}
	else
	{
		m_Option = ESelectedOption::OPTION1;
		m_PosX = POS_LEFT;
	}

	GetOwner()->GetComponent<TextureComponent>()->SetPosition(m_PosX, POSY);
}

void MenuComponent::Confirm()
{
	GameManager::Get()->NextAction(m_Option);
}