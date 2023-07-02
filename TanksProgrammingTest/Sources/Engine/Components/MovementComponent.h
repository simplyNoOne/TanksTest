#pragma once

#include "EntityComponent.h"

#include "../Engine/EnumInfo.h"


class CollisionComponent;

class MovementComponent : public EntityComponent
{

public:
	MovementComponent(Entity* Owner);
	MovementComponent();

	virtual EntityComponent* Clone() const override { return new MovementComponent(*this); }

	virtual void LoadFromConfig(const nlohmann::json& Config);
	virtual void Initialize() override;
	virtual void Update(float DeltaTime) override;

	virtual void SetPosition(int PosX, int PosY) override { m_PosX = PosX; m_PosY = PosY; }

	void SetVelocityX(float VelX) { m_VelocityX = VelX; }
	void SetVelocityY(float VelY) { m_VelocityY = VelY; }
	void SetMovementDirection(EMovementDirection Direction) { m_MovementDirection = Direction; }
	void SetCanMove(bool CanMove) { m_CanMove = CanMove; }

	float GetVelocityX() { return m_VelocityX; }
	float GetVelocityY() { return m_VelocityY; }
	EMovementDirection GetMovementDirection() { return m_MovementDirection; }
	bool GetCanMove() { return m_CanMove; }

	void Move(int newX, int newY);
	void Move() { Move(m_PosX, m_PosY); }

	void UpdateSize();
	void ObstacleHit(CollisionComponent* Self, CollisionComponent* Obstacle, ECollisionSide Side);


protected:
	int m_PosX;
	int m_PosY;
	int m_Width;
	int m_Height;

	float m_VelocityX;
	float m_VelocityY;
	bool m_CanMove;

	EMovementDirection m_MovementDirection;
	ECollisionSide m_LastHitSide;
};