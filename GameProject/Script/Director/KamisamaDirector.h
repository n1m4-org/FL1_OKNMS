#pragma once

#include <ActorRef.h>
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

    TPROPERTY(Edit, Required, ActorWith = "Plate")
    Tako::ActorRef plateA_;
    
    TPROPERTY(Edit, Required, ActorWith = "Plate")
    Tako::ActorRef plateB_;

public:
    void BeginPlay() override;
    void Tick(float deltaTime) override;
    void LateTick(float deltaTime) override;

#ifdef TAKO_EDITOR
    bool DrawCustomImGui() override;
#endif

private:

    // かみさまを出現させる
    Kamisama* Spawn();

    // 退場済みのかみさまを削除する
    void RemoveExited();

    // かみさまをレーンに紐づける
    void AttachToLane(Kamisama* kamisama, Lane* lane);

    bool CanSpawn() const;

    bool ShouldExit(const Kamisama& kamisama) const;

private:

    // Kamisama.prefab.json から Spawn した Actor のスクリプト。Actor はシーンが持つ
    std::vector<Kamisama*> kamisamas_;

    std::vector<std::unique_ptr<Lane>> pLanes_;
};
