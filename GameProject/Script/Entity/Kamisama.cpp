#include "Kamisama.h"

#include <Actor.h>
#include <RandomEngine.h>

#include <algorithm>

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

void Kamisama::Tick(float deltaTime)
{
    OnUpdate(state_, deltaTime);
}

void Kamisama::Initialize(const Tako::Vector3& initPos)
{
    GetOwner()->GetTransform().translate = initPos;

    OnEnter(State::Wandering);
}

void Kamisama::BeginExit()
{
    ChangeState(State::Exiting);
}

void Kamisama::AttachToLane(const Tako::Vector3& target)
{
    // レーンを割り当て移動を開始する
    targetPosition_= target;
    targetPosition_.y = GetOwner()->GetTransform().translate.y; // Y座標は現在の位置を維持
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

void Kamisama::OnUpdate(State state, float deltaTime)
{
    switch (state)
    {
        case State::Wandering:
            UpdateWandering(deltaTime);
            break;
        case State::Moving:
            UpdateMoving(deltaTime);
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
    wanderStartPosition_= GetOwner()->GetTransform().translate;
    wanderTarget_ = SelectWanderTarget();
}

void Kamisama::UpdateWandering(float deltaTime)
{
    Tako::Vector3& translate = GetOwner()->GetTransform().translate;

    // 徘徊先に到達したかどうかを判定するための閾値
    static const float kWanderThreshold = 0.5f;

    // 閾値を超えたら新しい徘徊先を選択
    if ((translate - wanderTarget_).Length() < kWanderThreshold)
    {
        // 徘徊先に到達したら新しい徘徊先を選択
        wanderStartPosition_= translate;
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

    Tako::Vector3 newPos = translate + direction * (wanderSpeed_ * deltaTime);

    translate = newPos;

}

Tako::Vector3 Kamisama::SelectWanderTarget() const
{
    auto randomEngine = RandomEngine::GetInstance();

    Tako::Vector3 target = kWanderCenter;
    target.x += randomEngine->GetFloat(kWanderRectMin.x, kWanderRectMax.x);
    target.y = GetOwner()->GetTransform().translate.y; // 現在のY座標を維持
    target.z += randomEngine->GetFloat(kWanderRectMin.z, kWanderRectMax.z);

    return target;
}

void Kamisama::EnterMoving()
{
    // 移動状態に変更
    startPosition_= GetOwner()->GetTransform().translate;
    moveElapsed_ = 0.0f;
}

void Kamisama::UpdateMoving(float deltaTime)
{
    // レーンに向かって移動
    moveElapsed_ += deltaTime;

    float t = moveElapsed_ / moveDuration_;
    t = std::clamp(t, 0.0f, 1.0f);

    Tako::Vector3 newPos = Tako::Vector3::Lerp(startPosition_, targetPosition_, t);

    GetOwner()->GetTransform().translate = newPos;

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

    Tako::Vector3& translate = GetOwner()->GetTransform().translate;

    // 一旦適当に場外に
    translate.y -= 0.1f;

    if (translate.y < -10.0f)
    {
        ChangeState(State::Exited);
    }
}
