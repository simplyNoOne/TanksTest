#pragma once

#include "EntityComponent.h"
#include "../Engine.h"


class TextureComponent : public EntityComponent
{

public:
	TextureComponent(Entity* Owner);
	TextureComponent();

	virtual EntityComponent* Clone() const override { return new TextureComponent(*this); }

	virtual void LoadFromConfig(const nlohmann::json& Config) override;
	virtual void Initialize() override;
	virtual void UnInitialize() override;
	virtual void Draw() override;
	virtual void SetPosition(int PosX, int PosY) override;

	void SetTextureFromAssetName(const std::string& Name);
	void SetScale(int w, int h);
	void SetVisible(bool Visible);

	SDL_Rect& GetRectangle() { return m_Rectangle; }



private:
	SDL_Texture* m_Texture;
	SDL_Texture* m_HiddenTexture;
	SDL_Rect m_Rectangle;

	int m_RelativeX;
	int m_RelativeY;
};