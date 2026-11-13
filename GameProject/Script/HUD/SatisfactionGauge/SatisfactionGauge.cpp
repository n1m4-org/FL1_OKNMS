#include "SatisfactionGauge.h"

#include "Actor.h"
#include "MeshComponent.h"
#include "Model.h"

#include <algorithm>
#include <numbers>

#include "Camera.h"
#include "Renderer.h"

#ifdef _DEBUG
#include "ImGui.h"
#endif

void SatisfactionGauge::BeginPlay()
{
    RebuildRings();

    start_ = std::chrono::system_clock::now();
}

void SatisfactionGauge::Tick([[maybe_unused]] float deltaTime)
{
    // 要素の変更があったフレームのみリングのリビルド
    if (isRingDirty_)
    {
        RebuildRings();
        isRingDirty_ = false;
    }

    Tako::Transform& transform = GetOwner()->GetTransform();

    if (Tako::Camera* camera = *Tako::Renderer::GetInstance()->GetCamera()) {
        constexpr float kHalfPi = std::numbers::pi_v<float> * 0.5f;

        const Tako::Vector3 cameraRotate = camera->GetRotate();
        transform.rotate =
        {
            cameraRotate.x - kHalfPi,
            cameraRotate.y,
            cameraRotate.z
        };
    }

    Decay();

    // 背景は、ゲージと同じ向きと大きさで、ワールドの offset_ だけずらして置く
    if (Tako::Actor* background = GetOwner()->Resolve(background_))
    {
        Tako::Transform bg = GetOwner()->GetWorldTransform();
        bg.translate += offset_;
        background->SetWorldTransform(bg);
    }
}

#ifdef _DEBUG
bool SatisfactionGauge::DrawCustomImGui()
{
    isRingDirty_ |= ImGui::DragFloat2("Radius", &radius_.x, 0.1f, 0.f, radius_.y + 0.1f);

    ImGui::SeparatorText("Gauge");
    isRingDirty_ |= ImGui::DragFloat("Percentage", &percentage_, 1.f, 0.f, 100.f);

    if(ImGui::Button("Charge"))
    {
        isRingDirty_ = true;
        percentage_ += chargeBuf_;
        percentage_ = std::clamp(percentage_, 0.f, 100.f);
    }

    ImGui::SameLine();

    ImGui::DragFloat("##charge", &chargeBuf_, 1.f, 1.f, 99.f - percentage_);

    if(ImGui::Button("Reduce"))
    {
        isRingDirty_ = true;
        percentage_ -= reduceBuf_;
        percentage_ = std::clamp(percentage_, 0.f, 100.f);
    }

    ImGui::SameLine();

    ImGui::DragFloat("##reduce", &reduceBuf_, 1.f, 1.f, 99.f - percentage_);

    ImGui::SeparatorText("Decay");

    if (ImGui::Button("Amount"))
    {
        decay_ = decayBuf_;
    }

    ImGui::SameLine();

    ImGui::DragFloat("##decay", &decayBuf_, 1.f, 0.f, 99.f - percentage_);

    if (ImGui::Button("Interval"))
    {
        decayInterval_ = intervalBuf_;
    }

    ImGui::SameLine();

    ImGui::DragFloat("##interval", &intervalBuf_, 0.1f, 0.f, 100.f);

    // 保存する値（TPROPERTY）は変えていない
    return false;
}
#endif

void SatisfactionGauge::Charge(const float value)
{
    percentage_ += value;
}

void SatisfactionGauge::Reduce(const float value)
{
    percentage_ -= value;
}

void SatisfactionGauge::SetDecay(const float value, const float interval)
{
    decay_ = value;
    decayInterval_ = interval;
}

void SatisfactionGauge::RebuildRings()
{
    radius_.y = std::max(radius_.x, radius_.y);

    gaugeParams_.sweepAngleDeg = kMaxAngle_ * (percentage_ / 100.f);
    gaugeParams_.startAngleDeg = -kStartAngleOffset_ + kMaxAngle_/2.f - gaugeParams_.sweepAngleDeg/2.f;
    gaugeParams_.innerRadius = radius_.x;
    gaugeParams_.outerRadius = radius_.y;

    if (Tako::MeshComponent* mesh = GetOwner()->GetComponent<Tako::MeshComponent>())
    {
        mesh->SetGeneratedModel(Tako::PrimitiveBuilder::CreateRing(gaugeParams_));
    }

    backgroundParams_.startAngleDeg = -kStartAngleOffset_;
    backgroundParams_.innerRadius = radius_.x;
    backgroundParams_.outerRadius = radius_.y;

    if (Tako::MeshComponent* mesh = BackgroundMesh())
    {
        mesh->SetGeneratedModel(Tako::PrimitiveBuilder::CreateRing(backgroundParams_));
    }
}

void SatisfactionGauge::Decay()
{
    if(decay_ <= 0.f) return;
    if(decayInterval_ <= 0.5f) return;

    auto end = std::chrono::system_clock::now();

    if (decayInterval_ <= std::chrono::duration<float>(end - start_).count())
    {
        isRingDirty_ = true;

        percentage_ -= decay_;
        percentage_ = std::clamp(percentage_, 0.f, 100.f);

        start_ = std::chrono::system_clock::now();
    }
}

Tako::MeshComponent* SatisfactionGauge::BackgroundMesh() const
{
    const Tako::Actor* background = GetOwner()->Resolve(background_);
    return background ? background->GetComponent<Tako::MeshComponent>() : nullptr;
}
