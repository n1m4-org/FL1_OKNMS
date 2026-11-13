#include "Kamisama.h"

#include <FrameTimer.h>
#include <EnginePaths.h>

#include <RandomEngine.h>

using  namespace Tako;

namespace
{
    // 徘徊場所の中心座標と範囲
    static constexpr Vector3 kWanderCenter = { 0.0f, 0.0f, 0.0f };
    static constexpr Vector3 kWanderRectMin = { -30.0f, 0.0f, -10.0f };
    static constexpr Vector3 kWanderRectMax = { 30.0f, 0.0f, 20.0f };

    static constexpr float kWanderWaitTimeMin = 2.0f; // 徘徊中の待機時間の最小値
    static constexpr float kWanderWaitTimeMax = 5.0f; // 徘徊中の待機時間の最大値
}

void Kamisama::Initialize(const Tako::Vector3& initPos)
{
    pModel_ = std::make_unique<Object3d>();
    pModel_->Initialize();
    pModel_->SetModel(EnginePaths::ModelPath("white_cube.gltf"));
    pModel_->SetMaterialColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    pModel_->SetEnableLighting(true);

    transform_.scale = { 1.0f, 2.0f, 1.0f };
    transform_.rotate = { 0.0f, 0.0f, 0.0f };
    transform_.translate = initPos;

    OnEnter(State::Wandering);
}

void Kamisama::Update()
{
    OnUpdate(state_);

    pModel_->SetTransform(transform_);
    pModel_->Update();
}

void Kamisama::Draw()
{
    pModel_->Draw();
}

void Kamisama::BeginExit()
{
    ChangeState(State::Exiting);
}

void Kamisama::AttachToLane(const Tako::Vector3& target)
{
    // レーンを割り当て移動を開始する
    targetPosition_= target;
    targetPosition_.y = transform_.translate.y; // Y座標は現在の位置を維持
    ChangeState(State::Moving);
}

void Kamisama::OnEnter(State state)
{
    switch (state)
    {
        case Kamisama::State::Wandering:
            EnterWandering();
            break;
        case Kamisama::State::Moving:
            EnterMoving();
            break;
        case Kamisama::State::Eating:
            EnterEating();
            break;
        case Kamisama::State::Exiting:
            EnterExiting();
            break;
        case Kamisama::State::Exited:
        default:
            break;
    }
}

void Kamisama::OnUpdate(State state)
{
    switch (state)
    {
        case State::Wandering:
            UpdateWandering();
            break;
        case State::Moving:
            UpdateMoving();
            break;
        case State::Eating:
            UpdateEating();
            break;
        case State::Exiting:
            UpdateExiting();
            break;
        case State::Exited:
            // 何もしない
            break;
    }
}

void Kamisama::ChangeState(State newState)
{
    state_ = newState;
    OnEnter(state_);
}

void Kamisama::EnterWandering()
{
    wanderStartPosition_= transform_.translate;
    wanderTarget_ = SelectWanderTarget();
}

void Kamisama::UpdateWandering()
{
    const float deltaTime = Tako::FrameTimer::GetInstance()->GetDeltaTime();


    // 徘徊先に到達したかどうかを判定するための閾値
    static const float kWanderThreshold = 0.5f;

    // 閾値を超えたら新しい徘徊先を選択
    if ((transform_.translate - wanderTarget_).Length() < kWanderThreshold)
    {
        // 徘徊先に到達したら新しい徘徊先を選択
        wanderStartPosition_= transform_.translate;
        // wanderWaitStateを用意するのを検討
        if (wanderWaitElapsed_ == 0.0f)// 初回の待機時間を設定
            wanderWaitTime_ = RandomEngine::GetInstance()->GetFloat(kWanderWaitTimeMin, kWanderWaitTimeMax); // 2秒から5秒のランダムな待機時間
        wanderWaitElapsed_ += deltaTime;
        if (wanderWaitTime_ > wanderWaitElapsed_)
        {
            return; // 待機時間が経過していない場合は処理を終了
        }
        wanderWaitElapsed_ = 0.0f;
        wanderTarget_ = SelectWanderTarget();
        return;
    }

    // 通常の徘徊処理
    Tako::Vector3 direction = wanderTarget_ - wanderStartPosition_;
    direction = direction.Normalize();

    Tako::Vector3 newPos = transform_.translate + direction * (wanderSpeed_ * deltaTime);

    transform_.translate = newPos;

}

Tako::Vector3 Kamisama::SelectWanderTarget() const
{
    auto randomEngine = RandomEngine::GetInstance();

    Tako::Vector3 target = kWanderCenter;
    target.x += randomEngine->GetFloat(kWanderRectMin.x, kWanderRectMax.x);
    target.y = transform_.translate.y; // 現在のY座標を維持
    target.z += randomEngine->GetFloat(kWanderRectMin.z, kWanderRectMax.z);

    return target;
}

void Kamisama::EnterMoving()
{
    // 移動状態に変更
    startPosition_= transform_.translate;
    moveElapsed_ = 0.0f;
}

void Kamisama::UpdateMoving()
{
    // レーンに向かって移動
    moveElapsed_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();

    float t = moveElapsed_ / moveDuration_;
    t = std::clamp(t, 0.0f, 1.0f);

    Tako::Vector3 newPos = Tako::Vector3::Lerp(startPosition_, targetPosition_, t);

    transform_.translate = newPos;

    if (t >= 1.0f)
    {
        ChangeState(State::Eating);
    }
}

void Kamisama::EnterEating()
{}

void Kamisama::UpdateEating()
{
    // 食事中の処理
    // アニメーションやゲージの更新イベントなどをここで処理する
}

void Kamisama::EnterExiting()
{}

void Kamisama::UpdateExiting()
{
    // 退場中の処理
    // 退場したら ChangeState(State::Exited) を呼び出す

    // 一旦適当に場外に
    transform_.translate.y -= 0.1f;

    if (transform_.translate.y < -10.0f)
    {
        ChangeState(State::Exited);
    }
}



