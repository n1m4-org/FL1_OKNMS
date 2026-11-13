#pragma once
#include <chrono>

#include "ActorRef.h"
#include "PrimitiveBuilder.h"
#include "Script.h"
#include "Vector2.h"
#include "Vector3.h"

namespace Tako { class MeshComponent; }

/// <summary>
/// 満足度ゲージ。同じ Actor の Mesh にゲージのリングを、背景の Actor の Mesh に背景のリングを描かせ、カメラの方を向ける。
/// リングの形は割合で変わるのでコードで作る（Mesh の Model の欄は空のまま）。色・ライティング・影は、それぞれの Mesh の Rendering で決める
/// </summary>
TCLASS(Requires = "MeshComponent")
class SatisfactionGauge : public Tako::Script
{
    GENERATED_BODY()

    static constexpr float kMaxAngle_ = 270.f;
    static constexpr float kStartAngleOffset_ = (360.f - kMaxAngle_) / 2.f;

    TPROPERTY(Edit, Required, ActorWith = "MeshComponent", Tooltip = "背景のリングを描く Actor（MeshComponent を付ける。モデルはコードが作る）")
    Tako::ActorRef background_;

    float denominator = 0.f;
    float fraction_ = 0.f;

    Tako::Vector2 radius_ {0.3f, 0.5f };

    Tako::PrimitiveBuilder::RingParams gaugeParams_{
        .innerRadius = radius_.x,
        .outerRadius = radius_.y,
        .segments = 64,
        .startAngleDeg = 0.f,
        .sweepAngleDeg = 30.f
    };
    Tako::PrimitiveBuilder::RingParams backgroundParams_{
        .innerRadius = radius_.x,
        .outerRadius = radius_.y,
        .segments = 64,
        .startAngleDeg = 0.f,
        .sweepAngleDeg = kMaxAngle_
    };

    // 背景を、ゲージからワールドでずらす量
    Tako::Vector3 offset_ = { 0.f, 0.f, 0.1f};

    // メッシュ再生成は描画コマンド記録中に行えないため、次の Tick まで遅延させる
    bool isRingDirty_ = false;

    float percentage_ = 0.f;

    std::chrono::system_clock::time_point start_;
    float decay_ = 0.f;
    float decayInterval_ = 1.f;

#ifdef _DEBUG
    float chargeBuf_ = 0.f;
    float reduceBuf_ = 0.f;
    float decayBuf_ = 0.f;
    float intervalBuf_ = 0.f;
#endif

public:
    void BeginPlay() override;
    void Tick(float deltaTime) override;

#ifdef _DEBUG
    /// <summary>
    /// ゲージの調整（Inspector に出る）。位置と大きさは Actor の Transform、色は Mesh の Rendering で変える
    /// </summary>
    bool DrawCustomImGui() override;
#endif

    /** @brief パーセントを追加する
      * @param value
      */
    void Charge(float value);

    /** @brief パーセントを減らす
     * @param value
     */
    void Reduce(float value);

    /** @brief 自動で減る値の設定
     * @param value 値
     * @param interval 間隔
     */
    void SetDecay(float value, float interval);

private:
    void RebuildRings();
    void Decay();

    Tako::MeshComponent* BackgroundMesh() const;
};
