#pragma once

#include "../../Engine/Components/EntityComponent.h"

class TextureComponent;

typedef std::function<void(int value)> AxisInputCallback;
typedef std::function<void()> ActionInputCallback;

class PlayerInputComponent : public EntityComponent
{

public:
	PlayerInputComponent(Entity* Owner);
	PlayerInputComponent();

	virtual EntityComponent* Clone() const override { return new PlayerInputComponent(*this); }

	virtual void Update(float DeltaTime) override;

	void BindAxisUp1(AxisInputCallback Call) { f_MoveUp1 = Call; }
	void BindAxisRight1(AxisInputCallback Call) { f_MoveRight1 = Call; }
	void BindActionShoot1(ActionInputCallback Call) { f_Shoot1 = Call; }

	void BindAxisUp2(AxisInputCallback Call) { f_MoveUp2 = Call; }
	void BindAxisRight2(AxisInputCallback Call) { f_MoveRight2 = Call; }
	void BindActionShoot2(ActionInputCallback Call) { f_Shoot2 = Call; }

	void BindActionConfirm(ActionInputCallback Call) { f_Confirm = Call; }

private:

	AxisInputCallback f_MoveUp1;
	AxisInputCallback f_MoveRight1;
	ActionInputCallback f_Shoot1;

	AxisInputCallback f_MoveUp2;
	AxisInputCallback f_MoveRight2;
	ActionInputCallback f_Shoot2;

	ActionInputCallback f_Confirm;

	int m_LastCode1 = 0;
	int m_LastCode2 = 0;
};