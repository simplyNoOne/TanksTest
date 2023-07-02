#include "PlayerMovementComponent.h"

#include "../../Engine/Components/CollisionComponent.h"
#include "../../Engine/Components/TextureComponent.h"
#include "../../Engine/GameManager.h"

#include "../GameState.h"

#include "Engine.h"
#include "Entity.h"
#include "PlayerInputComponent.h"

PlayerMovementComponent::PlayerMovementComponent(Entity* Owner)
	: MovementComponent(Owner)
	, m_TextureComponentPtr(nullptr)
	, m_AccX(0)
	, m_AccY(0)
	, m_JustTurned(false)
{
}

PlayerMovementComponent::PlayerMovementComponent()
	: PlayerMovementComponent(nullptr)
{
}

void PlayerMovementComponent::Initialize()
{
	MovementComponent::Initialize();

	m_TextureComponentPtr = GetOwner()->GetComponent<TextureComponent>();

	PlayerInputComponent* PlayerInputComponentPtr = GetOwner()->GetComponent<PlayerInputComponent>();
	if (PlayerInputComponentPtr)
	{
		if (GameManager::Get()->GetPlayerID(GetOwner()) == EPlayer::PLAYER1)
		{
			PlayerInputComponentPtr->BindAxisUp1([this](int value) { MoveUp(value); });
			PlayerInputComponentPtr->BindAxisRight1([this](int value) { MoveRight(value); });

			if (GameManager::Get()->GetGameMode() == EGameMode::SINGLEPLAYER)		//in case of singleplayer, player has two sets of controls
			{
				PlayerInputComponentPtr->BindAxisUp2([this](int value) { MoveUp(value); });
				PlayerInputComponentPtr->BindAxisRight2([this](int value) { MoveRight(value); });

			}
		}
		else if (GameManager::Get()->GetPlayerID(GetOwner()) == EPlayer::PLAYER2)
		{
			PlayerInputComponentPtr->BindAxisUp2([this](int value) { MoveUp(value); });
			PlayerInputComponentPtr->BindAxisRight2([this](int value) { MoveRight(value); });
		}
	}

}

void PlayerMovementComponent::Update(float DeltaTime)
{
	if (m_JustTurned) //if a player turns, his position shouldn't be changed, only rotated, so this ensures that no movement happens
	{
		m_JustTurned = false;
		Move(m_PosX, m_PosY);		//so that all components correct their positions after the turn
		return;
	}
	if (!m_CanMove)
	{
		m_AccX = 0;
		m_AccY = 0;
		return;
	}


	if (m_AccX != 0)
	{
		m_VelocityX += m_AccX * DeltaTime;
		if (m_VelocityX > MAX_VEL)
		{
			m_VelocityX = MAX_VEL;
		}
		if (m_VelocityX < -MAX_VEL)
		{
			m_VelocityX = -MAX_VEL;
		}
	}
	else
	{
		m_VelocityX = 0;
	}
	if (m_AccY != 0)
	{
		m_VelocityY += m_AccY * DeltaTime;
		if (m_VelocityY > MAX_VEL)
		{
			m_VelocityY = MAX_VEL;
		}
		if (m_VelocityY < -MAX_VEL)
		{
			m_VelocityY = -MAX_VEL;
		}
	}
	else
	{
		m_VelocityY = 0;
	}
	MovementComponent::Update(DeltaTime);

}

void PlayerMovementComponent::MoveUp(int Value)
{
	if (m_TextureComponentPtr)
	{
		if (Value > 0)
		{
			m_VelocityY = 0;		//to prevent player from slipping when abruptly changing direction by 180degrees
			m_MovementDirection = EMovementDirection::UP;
			m_TextureComponentPtr->SetTextureFromAssetName("RedTankFront.png");
			UpdateSize();
			GetOwner()->GetComponent<CollisionComponent>()->SetRelative(8, 2);
		}
		else if (Value < 0)
		{
			m_VelocityY = 0;
			m_MovementDirection = EMovementDirection::DOWN;
			m_TextureComponentPtr->SetTextureFromAssetName("RedTankDown.png");
			UpdateSize();
			GetOwner()->GetComponent<CollisionComponent>()->SetRelative(8, 10);
		}
	}
	if (!m_CanMove)
	{
		if (GetOwner()->GetComponent<CollisionComponent>()->DirectionTowardSide(m_LastHitSide) != m_MovementDirection)
		{
			m_CanMove = true;
			m_JustTurned = true;
		}
	}
	m_AccY = ACC * (-Value);
	m_AccX = 0;
}

void PlayerMovementComponent::MoveRight(int Value)
{

	if (m_TextureComponentPtr)
	{
		if (Value > 0)
		{
			m_VelocityX = 0;
			m_MovementDirection = EMovementDirection::RIGHT;
			m_TextureComponentPtr->SetTextureFromAssetName("RedTankRight.png");
			UpdateSize();
			GetOwner()->GetComponent<CollisionComponent>()->SetRelative(11, 5);
		}
		else if (Value < 0)
		{
			m_VelocityX = 0;
			m_MovementDirection = EMovementDirection::LEFT;
			m_TextureComponentPtr->SetTextureFromAssetName("RedTankLeft.png");
			UpdateSize();
			GetOwner()->GetComponent<CollisionComponent>()->SetRelative(3, 5);
		}
	}
	if (!m_CanMove)
	{
		if (GetOwner()->GetComponent<CollisionComponent>()->DirectionTowardSide(m_LastHitSide) != m_MovementDirection)
		{
			m_CanMove = true;
			m_JustTurned = true;
		}
	}
	m_AccX = ACC * Value;
	m_AccY = 0;
}