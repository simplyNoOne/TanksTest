#include "PlayerInputComponent.h"

#include "Engine.h"



PlayerInputComponent::PlayerInputComponent(Entity* Owner)
	: EntityComponent(Owner)
{
}

PlayerInputComponent::PlayerInputComponent()
	: PlayerInputComponent(nullptr)
{
}

void PlayerInputComponent::Update(float DeltaTime)
{
	const std::vector<SDL_Event>& Events = Engine::Get()->GetEvents();

	for (const SDL_Event& Event : Events)
	{
		SDL_Scancode code = Event.key.keysym.scancode;
		switch (Event.type)
		{
		case SDL_KEYDOWN:
		{
			switch (code)
			{
			case SDL_SCANCODE_W:
				if (f_MoveUp2)
				{
					f_MoveUp2(1);
					m_LastCode2 = code;
				}
				break;
			case SDL_SCANCODE_UP:
				if (f_MoveUp1)
				{
					f_MoveUp1(1);
					m_LastCode1 = code;
				}
				break;
			case SDL_SCANCODE_A:
				if (f_MoveRight2) {
					f_MoveRight2(-1);
					m_LastCode2 = code;
				}
				break;
			case SDL_SCANCODE_LEFT:
				if (f_MoveRight1)
				{
					f_MoveRight1(-1);
					m_LastCode1 = code;
				}
				break;
			case SDL_SCANCODE_S:
				if (f_MoveUp2)
				{
					f_MoveUp2(-1);
					m_LastCode2 = code;
				}
				break;
			case SDL_SCANCODE_DOWN:
				if (f_MoveUp1)
				{
					f_MoveUp1(-1);
					m_LastCode1 = code;
				}
				break;
			case SDL_SCANCODE_D:
				if (f_MoveRight2) {
					f_MoveRight2(1);
					m_LastCode2 = code;
				}
				break;
			case SDL_SCANCODE_RIGHT:
				if (f_MoveRight1)
				{
					f_MoveRight1(1);
					m_LastCode1 = code;
				}
				break;
			case SDL_SCANCODE_LSHIFT:
				if (f_Shoot2) {
					f_Shoot2();
				}
				break;
			case SDL_SCANCODE_RSHIFT:
				if (f_Shoot1)
				{
					f_Shoot1();
				}
				break;
			case SDL_SCANCODE_RETURN:
			case SDL_SCANCODE_KP_ENTER:
				if (f_Confirm)
				{
					f_Confirm();
				}
				break;
			default:
				break;
			}
			break;
		}
		case SDL_KEYUP:
		{
			if (m_LastCode1 != code && m_LastCode2 != code)
			{
				break;
			}
			switch (code)
			{
			case SDL_SCANCODE_W:
			case SDL_SCANCODE_S:
				if (f_MoveUp2) {
					f_MoveUp2(0);
				}
				break;
			case SDL_SCANCODE_UP:
			case SDL_SCANCODE_DOWN:
				if (f_MoveUp1)
				{
					f_MoveUp1(0);
				}
				break;
			case SDL_SCANCODE_A:
			case SDL_SCANCODE_D:
				if (f_MoveRight2) {
					f_MoveRight2(0);
				}
				break;
			case SDL_SCANCODE_LEFT:
			case SDL_SCANCODE_RIGHT:
				if (f_MoveRight1)
				{
					f_MoveRight1(0);
				}
				break;
			default:
				break;
			}
		}
		default:
			break;
		}

	}

}