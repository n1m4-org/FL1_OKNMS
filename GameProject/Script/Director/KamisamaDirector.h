#pragma once

#include <memory>
#include <vector>

#include <Script.h>

#include "Source/Director/Lane.h"

class Kamisama;

/// <summary>
/// かみさまの出現、退場管理を行うクラス。
/// Kamisama の Tick より先にレーンを割り当てるので、呼ぶ順を前にしている
/// </summary>
TCLASS(Order = -1)
class KamisamaDirector : public Tako::Script
{
    GENERATED_BODY()

public:
    void BeginPlay() override;
    void Tick(float deltaTime) override;
    void LateTick(float deltaTime) override;

private:

    // かみさまを出現させる
    Kamisama* Spawn();

    // かみさまの退場処理を開始する
    void BeginExit();

    // 退場済みのかみさまを削除する
    void RemoveExited();

    // かみさまをレーンに紐づける
    void AttachToLane(Kamisama* kamisama, Lane* lane);

    bool CanSpawn() const;

private:

    // Kamisama.prefab.json から Spawn した Actor のスクリプト。Actor はシーンが持つ
    std::vector<Kamisama*> kamisamas_;

    std::vector<std::unique_ptr<Lane>> pLanes_;


};
