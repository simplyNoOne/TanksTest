#pragma once

#include "EntityComponent.h"

class RootComponent : public EntityComponent
{

public:
	RootComponent(Entity* Owner);
	RootComponent();

	virtual EntityComponent* Clone() const override { return new RootComponent(*this); }

	virtual void LoadFromConfig(const nlohmann::json& Config) override;
	virtual void SetPosition(int PosX, int PosY) override;

	void SetEntityPosition(int PosX, int PosY);

	void SetX(int X) { m_X = X; }
	void SetY(int Y) { m_Y = Y; }

	int GetX() { return m_X; }
	int GetY() { return m_Y; }


private:
	int m_X;
	int m_Y;

};