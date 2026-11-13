#include "SatisfactionGauge.h"

#include "Model.h"

#include <numbers>

#include "Camera.h"
#include "Object3dbasic.h"

#ifdef _DEBUG
#include "ImGui.h"
#endif

void SatisfactionGauge::Initialize()
{
    gauge_ = std::make_unique<Tako::Object3d>();
    gauge_->Initialize();

    background_ = std::make_unique<Tako::Object3d>();
    background_->Initialize();

    RebuildRings();

    start_ = std::chrono::system_clock::now();
}

void SatisfactionGauge::Update()
{
    if (isRingDirty_)
    {
        RebuildRings();
        isRingDirty_ = false;
    }

    if (Tako::Camera* camera = *Tako::Object3dBasic::GetInstance()->GetCamera()) {
        constexpr float kHalfPi = std::numbers::pi_v<float> * 0.5f;
    
        const Tako::Vector3 cameraRotate = camera->GetRotate();
        transform_.rotate = 
        {
            cameraRotate.x - kHalfPi,
            cameraRotate.y,
            cameraRotate.z
        };
    }
    
    Decay();

    Tako::Transform bg = transform_;
    bg.translate += offset_;
    background_->SetTransform(bg);
    gauge_->SetTransform(transform_);

    background_->Update();
    gauge_->Update();
}

void SatisfactionGauge::Draw() const 
{
    background_->Draw();
    gauge_->Draw();
}

void SatisfactionGauge::Debug()
{
#ifdef _DEBUG

    ImGui::SeparatorText("Transform");
    ImGui::DragFloat3("Translate", &transform_.translate.x, 0.01f);

    constexpr float kRadToDeg = 180.f / std::numbers::pi_v<float>;
    Tako::Vector3 rotateDeg = transform_.rotate * kRadToDeg;
    if (ImGui::DragFloat3("Rotate", &rotateDeg.x, 1.f, -360.f, 360.f, "%.1f deg")) {
        transform_.rotate = rotateDeg / kRadToDeg;
    }

    ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);

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

    ImGui::SeparatorText("Color");
    if (ImGui::ColorEdit4("Gauge##Color", &gaugeColor_.x)) {
        gauge_->SetMaterialColor(gaugeColor_);
    }
    if (ImGui::ColorEdit4("Background##Color", &backgroundColor_.x)) {
        background_->SetMaterialColor(backgroundColor_);
    }

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

#endif
}

void SatisfactionGauge::Charge(const float _)
{
    percentage_ += _;
}

void SatisfactionGauge::Reduce(const float _)
{
    percentage_ -= _;
}

void SatisfactionGauge::SetDecay(const float _value, const float _interval) 
{
    decay_ = _value;
    decayInterval_ = _interval;
}

void SatisfactionGauge::RebuildRings()
{
	radius_.y = std::max(radius_.x, radius_.y);

    gaugeParams_.sweepAngleDeg = kMaxAngle_ * (percentage_ / 100.f);
    gaugeParams_.startAngleDeg = -kStartAngleOffset_ + kMaxAngle_/2.f - gaugeParams_.sweepAngleDeg/2.f;
	gaugeParams_.innerRadius = radius_.x;
	gaugeParams_.outerRadius = radius_.y;

    gauge_->SetModel(Tako::PrimitiveBuilder::CreateRing(gaugeParams_));
    gauge_->SetEnableLighting(false);
    gauge_->SetMaterialColor(gaugeColor_);

    backgroundParams_.startAngleDeg = -kStartAngleOffset_;
    backgroundParams_.innerRadius = radius_.x;
	backgroundParams_.outerRadius = radius_.y;
    background_->SetModel(Tako::PrimitiveBuilder::CreateRing(backgroundParams_));
    background_->SetEnableLighting(false);
    background_->SetMaterialColor(backgroundColor_);
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
