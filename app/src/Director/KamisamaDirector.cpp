#include "KamisamaDirector.h"

#include <src/Entity/Kamisama.h>
#include <RandomEngine.h>

// 仮
#include <Input.h>

constexpr static int kMaxKamisamaCount = 10;

// スポーン範囲(x, z)の範囲
constexpr float kSpawnRangeX = 15.0f;
constexpr float kSpawnRangeZ = 10.0f;

// フロアを基準としたプレイヤーのy座標
constexpr float kPlayerY = 2.0f;

KamisamaDirector::KamisamaDirector() = default;
KamisamaDirector::~KamisamaDirector()= default;

void KamisamaDirector::Initialize()
{
    kamisamas_.clear();
    kamisamas_.reserve(kMaxKamisamaCount);

    pLanes_.clear();
    pLanes_.reserve(2);

    auto lane1 = std::make_unique<Lane>();
    lane1->Initialize({ -5.0f, 0, -20.0f });
    pLanes_.push_back(std::move(lane1));

    auto lane2 = std::make_unique<Lane>();
    lane2->Initialize({ 5.0f, 0, -20.0f });
    pLanes_.push_back(std::move(lane2));

    Spawn();
    Spawn();
}

void KamisamaDirector::Update()
{
    for (auto& l : pLanes_)
    {
        if (!l->IsAttached())
        {
            int32_t maxLoopCount = kMaxKamisamaCount * 2;
            while (true)
            {
                if (kamisamas_.size() == 0)
                {
                    break;
                }

                int randomIndex = Tako::RandomEngine::GetInstance()->GetInt(0, static_cast<int>(kamisamas_.size()) - 1);
                auto& kamisama = kamisamas_[randomIndex];
                if (kamisama && kamisama->IsWandering())// 徘徊中のかみさまを見つけたらレーンに紐づける
                {
                    AttachToLane(kamisama.get(), l.get());
                    break;
                }
                // 無限ループ防止のため、最大ループ回数を設定
                maxLoopCount--;
                if (maxLoopCount <= 0)
                {
                    break;
                }
            }
        }
    }

    if (Tako::Input::GetInstance()->TriggerKey(DIK_L))
    {
        Spawn();
    }
    if (Tako::Input::GetInstance()->TriggerKey(DIK_K))
    {
        BeginExit();
    }

    for (auto& kamisama : kamisamas_)
    {
        kamisama->Update();
    }

    RemoveExited();

}

void KamisamaDirector::Draw()
{
    for (auto& kamisama : kamisamas_)
    {
        kamisama->Draw();
    }
}

Kamisama* KamisamaDirector::Spawn()
{
    if (!CanSpawn())
    {
        return nullptr;
    }

    auto kamisama = std::make_unique<Kamisama>();

    Tako::Vector3 spawnPos = { Tako::RandomEngine::GetInstance()->GetFloat(-kSpawnRangeX, kSpawnRangeX),
                            kPlayerY,
                            Tako::RandomEngine::GetInstance()->GetFloat(-kSpawnRangeZ, kSpawnRangeZ) };

    // 返り値のため生ポを保持
    Kamisama* kamisamaPtr = kamisama.get();
    kamisama->Initialize(spawnPos);

    kamisamas_.push_back(std::move(kamisama));

    return kamisamaPtr;
}

void KamisamaDirector::BeginExit()
{
    for (auto& kamisama : kamisamas_)
    {
        for (auto& l : pLanes_)
        {
            if (l->IsAttachedTo(kamisama.get()))
            {
                l->DetachKamisama();
                kamisama->BeginExit();
            }
        }
    }
}

void KamisamaDirector::RemoveExited()
{
    std::erase_if(kamisamas_, [](const std::unique_ptr<Kamisama>& kamisama)
                  {
                      return kamisama->IsExited();
                  });
}

void KamisamaDirector::AttachToLane(Kamisama* kamisama, Lane* lane)
{
    if (kamisama && lane)
    {
        lane->AttachKamisama(kamisama);
        kamisama->AttachToLane(lane->GetPosition());
    }
}

bool KamisamaDirector::CanSpawn() const
{
    // かみさまの最大出現数に達していない場合は出現可能
    if (kamisamas_.size() < kMaxKamisamaCount)
    {
        return true;
    }

    return false;
}
