#pragma once

#include "EntityComponent.h"
#include "../Engine/Collidable.h"
#include "../Engine/EnumInfo.h"
#include "Engine.h"
#include <unordered_set>
#include <bitset>


class TextureComponent;
class SDL_Texture;
class CollisionComponent : public EntityComponent, public Collidable
{
public:
	CollisionComponent(Entity* Owner);
	CollisionComponent();

	virtual EntityComponent* Clone() const override { return new CollisionComponent(*this); }

	virtual void Initialize() override;
	virtual void UnInitialize() override;
	virtual void LoadFromConfig(const nlohmann::json& Config) override;

	void SetPosition(int PosX, int PosY);
	void SetRelative(int X, int Y);
	void SetSize(int W, int H) { m_Bounds.w = W; m_Bounds.h = H; }

	SDL_Rect* GetBounds() { return &m_Bounds; }


private:
	virtual void Hit(CollisionComponent* OtherCollider) override;
	ECollisionSide CheckCollisionSide(CollisionComponent* OtherCollider);


private:
	SDL_Rect  m_Bounds;
	int m_RelativeX;
	int m_RelativeY;

};
