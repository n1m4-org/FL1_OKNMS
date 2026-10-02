#include "Game.h"

#include "Input.h"
#include "SceneManager.h"
#include "Scene/SceneFactory.h"

using namespace Tako;

void Game::Initialize()
{
  winApp_->SetWindowSize(1920, 1080);

  winApp_->SetWindowTitle(L"FL1_OKNMS");

  TakoFramework::Initialize();

  // シーンの初期化
  sceneFactory_ = std::make_unique<SceneFactory>();
  SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
  SceneManager::GetInstance()->ChangeScene("game", 0.0f);
}

void Game::Finalize()
{
  TakoFramework::Finalize();
}

void Game::Update()
{
  // F11キーでフルスクリーン切り替え
  if (Input::GetInstance()->TriggerKey(DIK_F11)) {
    ToggleFullScreen();
  }
  TakoFramework::Update();
}

void Game::Draw()
{
  TakoFramework::Draw();
}