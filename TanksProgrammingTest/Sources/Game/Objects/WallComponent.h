#pragma once

#include "../../Engine/Components/EntityComponent.h"
#include "../Engine/EnumInfo.h"

class CollisionComponent;
class WallComponent : public EntityComponent
{

public:
	WallComponent(Entity* Owner);
	WallComponent();

	virtual EntityComponent* Clone() const override { return new WallComponent(*this); }

	virtual void LoadFromConfig(const nlohmann::json& Config) override;
	virtual void Initialize() override;

	EWallType GetType() { return m_WallType; }

	void BeginOverlap(CollisionComponent* Self, CollisionComponent* Other);
	void WallShot();


private:
	EWallType m_WallType;
	bool m_WasHit;


};