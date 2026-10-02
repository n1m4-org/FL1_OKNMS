#include "GameScene.h"

#include "LineRenderer.h"
#include "GPUParticle.h"
#include "Input.h"
#include "Object3dBasic.h"
#include "SceneManager.h"
#include "SpriteBasic.h"

#ifdef _DEBUG
#include"ImGui.h"
#endif

using namespace Tako;

void GameScene::Initialize()
{
  /// ================================== ///
  ///              初期化処理              ///
  /// ================================== ///


}


void GameScene::Finalize()
{

}

void GameScene::Update()
{
  /// ================================== ///
  ///              更新処理               ///
  /// ================================== ///



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