#pragma once

#include <Script.h>

#include <system/EventListener.h>


/// 制限時間クラス
/// ゲームの制限時間をカウントするクラス
/// 時間が０になったら行う処理は別のクラスが担当する

TCLASS()
class TimeLimit : public Tako::Script
{
    GENERATED_BODY()

public:
    TENUM()
    enum class State { Ready, Running, Paused };

    void BeginPlay() override;
    void Tick(float deltaTime) override;


#ifdef TAKO_EDITOR
    bool DrawCustomImGui() override;
#endif // TAKO_EDITOR


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

    void PublishEventIfFinished();

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
