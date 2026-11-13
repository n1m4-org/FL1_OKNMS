#include "TimeLimit.h"

#include <imgui.h>
#include <TextureManager.h>
#include <constant/ResourcePath.h>

void TimeLimit::Initialize()
{
    // テクスチャの読み込み
    this->LoadTexture();

    nv_ = std::make_unique<NumericView>();
    nv_->Initialize(textureIDs_, "TimeLimit");
}

void TimeLimit::Update()
{
    nv_->Update();
}

void TimeLimit::ImGui()
{

}

void TimeLimit::LoadTexture()
{
    for (int i = 0; i < 10; i++)
    {
        auto fileName = Path::Image::numbers[i];
        Tako::TextureManager::GetInstance()->LoadTexture(fileName);
    }
}

void TimeLimit::CollectTextureIDs()
{
    for (int i = 0; i < 10; i++)
    {
        auto fileName = Path::Image::numbers[i];
        auto handle = Tako::TextureManager::GetInstance()->GetSRVIndex(fileName);
        textureIDs_[i] = handle;
    }
}
