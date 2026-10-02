#include "SceneFactory.h"
#include "GameScene.h"

#ifdef _DEBUG
#include "DebugUIManager.h"
#endif

using namespace Tako;

std::unique_ptr<Tako::BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
  if (sceneName == "game") {
    return std::make_unique<GameScene>();
  }

#ifdef _DEBUG
  DebugUIManager::GetInstance()->AddLog("Unknown scene name: " + sceneName, DebugUIManager::LogType::Error);
#endif

  return nullptr;
}