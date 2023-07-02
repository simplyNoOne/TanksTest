#pragma once

#include "../../Engine/Components/EntityComponent.h"
#include "../Engine/EnumInfo.h"

class CollisionComponent;

typedef std::function<void()> BaseDestoryedCallback;

class PlayerBaseComponent : public EntityComponent
{

public:
	PlayerBaseComponent(Entity* Owner);
	PlayerBaseComponent();

	virtual EntityComponent* Clone() const override { return new PlayerBaseComponent(*this); }

	virtual void LoadFromConfig(const nlohmann::json& Config) override;
	virtual void Initialize() override;

	void BindToBaseDestroyed(BaseDestoryedCallback Call) { f_BaseDestroyed = Call; }

	void BeginOverlap(CollisionComponent* Self, CollisionComponent* Other);

	void BaseHit();

private:
	EWallType m_WallType;
	bool m_WasHit;

	BaseDestoryedCallback f_BaseDestroyed;

};