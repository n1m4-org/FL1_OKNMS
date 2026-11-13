#include "GameScene.h"

#include "LineRenderer.h"
#include "GPUParticle.h"
#include "Input.h"
#include "Object3dBasic.h"
#include "SceneManager.h"
#include "SpriteBasic.h"

#ifdef _DEBUG
#include "ImGui.h"
#include "DebugUIManager.h"
#endif

using namespace Tako;

void GameScene::Initialize()
{
  /// ================================== ///
  ///              初期化処理              ///
  /// ================================== ///
  
  emitters_ = std::make_unique<EmitterManager>(GPUParticle::GetInstance());
    
    gauge_ = std::make_unique<SatisfactionGauge>();
    gauge_->Initialize();

#ifdef _DEBUG
    DebugUIManager::GetInstance()->SetEmitterManager(emitters_.get());
    DebugUIManager::GetInstance()->RegisterGameObject("Gauge", [this]{ gauge_->Debug(); });
#endif

}


void GameScene::Finalize()
{
#ifdef _DEBUG
    DebugUIManager::GetInstance()->UnregisterGameObject("Gauge");
#endif

}

void GameScene::Update()
{
  /// ================================== ///
  ///              更新処理               ///
  /// ================================== ///

    gauge_->Update();

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
  //------------------背景Spriteの描画------------------//
  // スプライト共通描画設定
  SpriteBasic::GetInstance()->SetCommonRenderSetting();




  //-------------------Modelの描画-------------------//
  // 3Dモデル共通描画設定
  Object3dBasic::GetInstance()->SetCommonRenderSetting();

    gauge_->Draw();



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