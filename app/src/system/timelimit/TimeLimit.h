#pragma once
#include <memory>
#include <font/NumericView.h>

class TimeLimit
{
public:
    void Initialize();
    void Update();

    void ImGui();

private:
    void LoadTexture();
    void CollectTextureIDs();

    std::unique_ptr<NumericView> nv_;
    std::span<NumericView::TextureIDType> textureIDs_;
};
