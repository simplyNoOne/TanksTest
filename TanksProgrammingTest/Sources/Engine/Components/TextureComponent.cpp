#include "TextureComponent.h"

#include "Engine.h"
#include "ResourceManager.h"

TextureComponent::TextureComponent(Entity* Owner)
	: EntityComponent(Owner)
	, m_Texture(nullptr)
	, m_HiddenTexture(nullptr)
	, m_RelativeX(0)
	, m_RelativeY(0)
	, m_Rectangle{ 0,0,0,0 }
{
}

TextureComponent::TextureComponent()
	: TextureComponent(nullptr)
{
}

void TextureComponent::LoadFromConfig(const nlohmann::json& Config)
{
	const std::string& TextureName = Config.value("Texture", "");
	if (!TextureName.empty())
	{
		SetTextureFromAssetName(TextureName);
	}

	m_Rectangle.w = Config.value("Width", 10);
	m_Rectangle.h = Config.value("Height", 10);
	m_RelativeX = Config.value("RelativeX", 0);
	m_RelativeY = Config.value("RelativeY", 0);
}

void TextureComponent::Initialize()
{
}

void TextureComponent::UnInitialize()
{
	m_HiddenTexture = nullptr;
	m_Texture = nullptr;
}

void TextureComponent::Draw()
{
	SDL_RenderCopy(Engine::Get()->GetRenderer(), m_Texture, nullptr, &m_Rectangle);
}

void TextureComponent::SetPosition(int PosX, int PosY)
{
	m_Rectangle.x = PosX + m_RelativeX;
	m_Rectangle.y = PosY + m_RelativeY;
}

void TextureComponent::SetTextureFromAssetName(const std::string& Name)
{
	m_Texture = Engine::Get()->GetResourceManager()->GetTextureByName(Name);
	m_HiddenTexture = m_Texture;
}



void TextureComponent::SetScale(int w, int h)
{
	m_Rectangle.w = w;
	m_Rectangle.h = h;
}

void TextureComponent::SetVisible(bool Visible)
{
	m_Texture = Visible ? m_HiddenTexture : nullptr;
}