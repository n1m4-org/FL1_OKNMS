#pragma once

#include <Script.h>

TCLASS()
class TimeLimit : public Tako::Script
{
    GENERATED_BODY()

public:
    TENUM()
    enum class State { Ready, Running, Paused };


    void Tick(float deltaTime) override;


#ifdef _DEBUG
    bool DrawCustomImGui() override;
#endif // _DEBUG


    /// カウントダウンの操作
    void Start();
    void Pause();
    void Reset();


    // 残り時間の取得
    float GetRemainingTime() const { return remainingTime_; }

    // 残り時間の追加
    void AddRemainingTime(float time);

private:
    // 残り時間をへらす
    void DecreaseTime(float time);

    // 制限時間を終えたらカウントを停止
    void CountStopIfFinished();

    // テキストの更新
    void UpdateText();


    TPROPERTY(Edit)
    float maxLimitTime_ = 60.0f;

    TPROPERTY(Visible)
    float remainingTime_ = 0.0f;

    TPROPERTY(Visible)
    State state_ = State::Ready; 


    // UI に反映するかどうかのフラグ。残り時間が変わったら true にする
    bool isDirty_ = false;
};