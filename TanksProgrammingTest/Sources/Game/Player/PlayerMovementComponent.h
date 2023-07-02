#pragma once

#include "../../Engine/Components/MovementComponent.h"

class TextureComponent;

class PlayerMovementComponent : public MovementComponent
{

public:
	PlayerMovementComponent(Entity* Owner);
	PlayerMovementComponent();

	virtual EntityComponent* Clone() const override { return new PlayerMovementComponent(*this); }

	virtual void Initialize() override;
	virtual void Update(float DeltaTime) override;

	void MoveUp(int Value);
	void MoveRight(int Value);

private:
	const int ACC = 900;
	const int MAX_VEL = 250;

	TextureComponent* m_TextureComponentPtr;

	int m_AccX;
	int m_AccY;

	bool m_JustTurned;

};