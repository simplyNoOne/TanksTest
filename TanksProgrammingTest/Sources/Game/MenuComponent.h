#pragma once

#include "../Engine/Components/EntityComponent.h"
#include "../Engine/EnumInfo.h"
#include <SDL.h>


class SDL_Rect;
class MenuComponent : public EntityComponent
{

public:
	MenuComponent(Entity* Owner);
	MenuComponent();

	virtual EntityComponent* Clone() const override { return new MenuComponent(*this); }

	virtual void Initialize() override;
	virtual void Draw()override;

	void SwitchChoice(int Value);
	void Confirm();

private:
	const int POSY = 215;
	const int POS_LEFT = 140;
	const int POS_RIGHT = 540;

	ESelectedOption m_Option;
	int m_PosX;

	SDL_Texture* m_ResultsMessage;
	SDL_Rect m_MessageBounds;

};