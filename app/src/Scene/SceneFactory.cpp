#include "SceneFactory.h"
#include "GameScene.h"

#ifdef _DEBUG
#include "DebugUIManager.h"
#endif

using namespace Tako;

namespace {
    struct SceneEntry {
        const char* name;
        std::unique_ptr<BaseScene>(*create)();
    };

    // シーンを増やすときはここに 1 行足す
    const SceneEntry kScenes[] = {
      { "game", []() -> std::unique_ptr<BaseScene> { return std::make_unique<GameScene>(); } },
    };
}

std::unique_ptr<Tako::BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    for (const SceneEntry& entry : kScenes) {
        if (sceneName == entry.name) {
            return entry.create();
        }
    }

#ifdef _DEBUG
    DebugUIManager::GetInstance()->AddLog("Unknown scene name: " + sceneName, DebugUIManager::LogType::Error);
#endif

    return nullptr;
}

std::vector<std::string> SceneFactory::GetSceneNames() const
{
    std::vector<std::string> names;
    for (const SceneEntry& entry : kScenes) {
        names.emplace_back(entry.name);
    }
    return names;
}
