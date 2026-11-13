#pragma once

#include <Script.h>
#include <Vector3.h>

/// <summary>
/// かみさま。徘徊し、レーンを割り当てられるとそこへ移動する。
/// モデル・大きさ・色は、プレハブ（Kamisama.prefab.json）の Mesh と Transform が持つ
/// </summary>
TCLASS()
class Kamisama : public Tako::Script
{
    GENERATED_BODY()

private:
    // state 徘徊中/移動中/食事中/退場中/退場済
    enum class State
    {
        Wandering,
        Moving,
        Eating,
        Exiting,
        Exited
    };

public:
    void Tick(float deltaTime) override;

    /// <summary>
    /// 出現位置を決めて徘徊を始める。Spawn した直後に呼ぶ
    /// </summary>
    void Initialize(const Tako::Vector3& initPos = { 0.0f, 2.0f, 0.0f });// 初期値yは2.0fで、モデルの原点を考慮

    void BeginExit();

    void AttachToLane(const Tako::Vector3& target);
    bool IsWandering() const { return state_ == State::Wandering; }
    bool IsAttachedToLane() const { return state_ == State::Moving || state_ == State::Eating; }
    bool IsExited() const { return state_ == State::Exited; }

private:

    void OnEnter(State state);
    void OnUpdate(State state, float deltaTime);

    void ChangeState(State newState);

    void EnterWandering();
    void UpdateWandering(float deltaTime);

    Tako::Vector3 SelectWanderTarget() const;

    void EnterMoving();
    void UpdateMoving(float deltaTime);

    void EnterEating();
    void UpdateEating();

    void EnterExiting();
    void UpdateExiting();


private:

    State state_ = State::Wandering;

    /// 徘徊中のパラメータ
    // 徘徊前の位置
    Tako::Vector3 wanderStartPosition_ = {};
    // 徘徊先の位置
    Tako::Vector3 wanderTarget_ = {};
    // 移動の速さ
    float wanderSpeed_ = 1.0f;
    // 次の徘徊を始めるまでの時間
    float wanderWaitTime_ = 1.0f;
    float wanderWaitElapsed_ = 0.0f;

    /// 移動中のパラメータ
    // 移動前の位置
    Tako::Vector3 startPosition_ = {};
    // 移動先
    Tako::Vector3 targetPosition_ = {};
    // 移動時間
    float moveDuration_ = 2.0f;
    float moveElapsed_ = 0.0f;
};
