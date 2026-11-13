#pragma once
#include <memory>

#include "Object3d.h"
#include "PrimitiveBuilder.h"
#include "Transform.h"
#include "Vector4.h"

class SatisfactionGauge
{
    const float MAX_ANGLE = 270.f;
    const float START_ANGLE_OFFSET = (360.f - MAX_ANGLE) / 2.f; 

    std::unique_ptr<Tako::Object3d> gauge_;
    std::unique_ptr<Tako::Object3d> background_;
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
        .sweepAngleDeg = MAX_ANGLE
    };
    Tako::Vector4 gaugeColor_ = { 1.f, 1.f, 1.f, 1.f };
    Tako::Vector4 backgroundColor_ = { 0.3f, 0.3f, 0.3f, 1.f };

    Tako::Transform transform_ = {
        .scale = { 1.f, 1.f, 1.f },
        .rotate = { 0.f, 0.f, 0.f },
        .translate = { 0.f, 0.f, 0.f }
    };

    Tako::Vector3 offset_ = { 0.f, 0.f, 0.1f};

    // メッシュ再生成は描画コマンド記録中に行えないため、次の Update まで遅延させる
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
    void Initialize();
    void Update();
    void Draw() const;
    void Debug();

    void Charge(float _);
    void Reduce(float _);
	void SetDecay(float _value, float _interval);

private:
    void RebuildRings();
	void Decay();
};
