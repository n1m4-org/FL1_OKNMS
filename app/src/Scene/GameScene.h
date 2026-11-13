#pragma once
#include "BaseScene.h"
#include "EmitterManager.h"
#include "HUD/SatisfactionGauge/SatisfactionGauge.h"

#include <Object3d.h>

#include <memory>

#include <src/Director/KamisamaDirector.h>

/// <summary>
/// サンプルシーンクラス
/// TakoEngineのデフォルトシーンの構成を参考にしてください。
/// </summary>
class GameScene : public Tako::BaseScene
{
public: // メンバ関数

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;
    void DrawWithoutEffect() override;

    /// <summary>
    /// ImGuiの描画
    /// </summary>
    void DrawImGui() override;

private: // メンバ変数
	  std::unique_ptr<Tako::EmitterManager> emitters_;
  	std::unique_ptr<SatisfactionGauge> gauge_;
    std::unique_ptr<KamisamaDirector> pKamisamaDirector_= nullptr;
    std::unique_ptr<Tako::Object3d> pFloor_= nullptr;
};
