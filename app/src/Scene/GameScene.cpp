#include "GameScene.h"

#include "LineRenderer.h"
#include "GPUParticle.h"
#include "Input.h"
#include "Object3dBasic.h"
#include "SceneManager.h"
#include "ShadowRenderer.h"
#include "SpriteBasic.h"

#include <PrimitiveBuilder.h>
#include <RandomEngine.h>
#include <EnginePaths.h>
#include <Model.h>

#ifdef _DEBUG
#include"ImGui.h"
#endif

using namespace Tako;

void GameScene::Initialize()
{
    /// ================================== ///
    ///              初期化処理              ///
    /// ================================== ///


    pFloor_ = std::make_unique<Tako::Object3d>();
    pFloor_->Initialize();
    pFloor_->SetModel(std::move(Tako::PrimitiveBuilder::CreatePlane()));
    pFloor_->SetScale({ 100.0f, 1.0f, 100.0f });
    pFloor_->SetEnableLighting(true);

    // 初期化
    pKamisamaDirector_ = std::make_unique<KamisamaDirector>();
    pKamisamaDirector_->Initialize();

}


void GameScene::Finalize()
{

}

void GameScene::Update()
{
    /// ================================== ///
    ///              更新処理               ///
    /// ================================== ///

    pFloor_->Update();
    pKamisamaDirector_->Update();

    if (Input::GetInstance()->TriggerKey(DIK_RETURN))
    {
        SceneManager::GetInstance()->ChangeScene("");
    }
}

void GameScene::Draw()
{
    /// ================================== ///
    ///              描画処理               ///
    /// ================================== ///

    //------------------シャドウマップ生成パス------------------//
    // シャドウ有効時はここでシャドウマップに書き込まないと、未書き込みのマップを参照して影だらけになる
    if (ShadowRenderer::GetInstance()->IsEnabled())
    {
        ShadowRenderer::GetInstance()->BeginShadowPass();

        // シャドウキャスターの描画
        pFloor_->Draw();
        pKamisamaDirector_->Draw();
        ShadowRenderer::GetInstance()->EndShadowPass();
    }

    //------------------背景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();




    //-------------------Modelの描画-------------------//
    // 3Dモデル共通描画設定
    Object3dBasic::GetInstance()->SetCommonRenderSetting();

    pFloor_->Draw();
    pKamisamaDirector_->Draw();

    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();



}

void GameScene::DrawWithoutEffect()
{
    /// ================================== ///
    ///              描画処理               ///
    /// ================================== ///

    //------------------背景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();




    //-------------------Modelの描画-------------------//
    // 3Dモデル共通描画設定
    Object3dBasic::GetInstance()->SetCommonRenderSetting();





    //------------------前景Spriteの描画------------------//
    // スプライト共通描画設定
    SpriteBasic::GetInstance()->SetCommonRenderSetting();




}

void GameScene::DrawImGui()
{
#ifdef _DEBUG

    /// ================================== ///
    ///             ImGuiの描画              ///
    /// ================================== ///


#endif // _DEBUG
}