#pragma once

#include <memory>
#include <vector>

#include "Lane.h"

class Kamisama;

/// <summary>
/// かみさまの出現、退場管理を行うクラス
/// </summary>
class KamisamaDirector
{
public:
    KamisamaDirector();
    ~KamisamaDirector();

    void Initialize();
    void Update();
    void Draw();

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

    std::vector<std::unique_ptr<Kamisama>> kamisamas_;

    std::vector<std::unique_ptr<Lane>> pLanes_;


};