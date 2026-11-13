#pragma once
#include <Script.h>

/// <summary>
/// キューに溜まったイベント（EventListener::Publish）を、フレームの最後に処理する。
/// イベントを使うシーンに 1 つ置く。全部のスクリプトの後に呼ばれるよう、呼ぶ順を最後にしている
/// </summary>
TCLASS(Order = 10000)
class EventDispatcher : public Tako::Script
{
    GENERATED_BODY()

public:
    void LateTick(float deltaTime) override;
};
