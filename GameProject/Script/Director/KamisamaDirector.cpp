#include "KamisamaDirector.h"


#include <Actor.h>
#include <BaseScene.h>
#include <Logger.h>
#include <RandomEngine.h>

// 仮
#include <Input.h>

#include "Script/Entity/Kamisama.h"
#include "Eating/Plate.h"

#ifdef TAKO_EDITOR
#include "imgui.h"
#endif

constexpr static int kMaxKamisamaCount = 10;

// スポーン範囲(x, z)の範囲
constexpr float kSpawnRangeX = 15.0f;
constexpr float kSpawnRangeZ = 10.0f;

// フロアを基準としたプレイヤーのy座標
constexpr float kPlayerY = 2.0f;

// resources/Json/Prefabs にあるかみさまのプレハブ
constexpr const char* kKamisamaPrefab = "Kamisama";

void KamisamaDirector::BeginPlay()
{
    kamisamas_.clear();
    kamisamas_.reserve(kMaxKamisamaCount);

    pLanes_.clear();
    pLanes_.reserve(2); 
    
    auto actor = GetOwner()->Resolve(plateA_);
    auto plate = actor ? actor->GetScript<Plate>() : nullptr;

    auto lane1 = std::make_unique<Lane>();
    lane1->Initialize({ -5.0f, 0, -20.0f }, plate);
    pLanes_.push_back(std::move(lane1));

    actor = GetOwner()->Resolve(plateB_);
    plate = actor ? actor->GetScript<Plate>() : nullptr;

    auto lane2 = std::make_unique<Lane>();
    lane2->Initialize({ 5.0f, 0, -20.0f }, plate);
    pLanes_.push_back(std::move(lane2));

    Spawn();
    Spawn();
}

void KamisamaDirector::Tick([[maybe_unused]] float deltaTime)
{
    for (auto& lane : pLanes_)
    {
        if (auto kamisama = lane->GetKamisama(); kamisama && ShouldExit(*kamisama))
        {
            kamisama->BeginExit();
        }

        if (!lane->IsAttached())
        {
            int32_t maxLoopCount = kMaxKamisamaCount * 2;
            while (true)
            {
                if (kamisamas_.empty())
                {
                    break;
                }

                int randomIndex = Tako::RandomEngine::GetInstance()->GetInt(0, static_cast<int>(kamisamas_.size()) - 1);
                Kamisama* kamisama = kamisamas_[randomIndex];
                if (kamisama && kamisama->IsWandering())// 徘徊中のかみさまを見つけたらレーンに紐づける
                {
                    AttachToLane(kamisama, lane.get());
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
}

void KamisamaDirector::LateTick([[maybe_unused]] float deltaTime)
{
    for(const auto& lane : pLanes_)
    {
        if(auto kamisama = lane->GetKamisama(); kamisama && kamisama->IsExited())
        {
            lane->DetachKamisama();
        }
    }
    RemoveExited();
}

Kamisama* KamisamaDirector::Spawn()
{
    if (!CanSpawn())
    {
        return nullptr;
    }

    Tako::Actor* actor = GetOwner()->GetScene()->Spawn(kKamisamaPrefab);
    Kamisama* kamisama = actor ? actor->GetScript<Kamisama>() : nullptr;
    if (!kamisama)
    {
        Tako::Logger::Log("KamisamaDirector: the prefab \"%s\" is missing or has no Kamisama script\n", kKamisamaPrefab);
        if (actor)
        {
            actor->Destroy();
        }
        return nullptr;
    }

    Tako::Vector3 spawnPos = { Tako::RandomEngine::GetInstance()->GetFloat(-kSpawnRangeX, kSpawnRangeX),
                            kPlayerY,
                            Tako::RandomEngine::GetInstance()->GetFloat(-kSpawnRangeZ, kSpawnRangeZ) };

    kamisama->Initialize(spawnPos);

    kamisamas_.push_back(kamisama);

    return kamisama;
}

void KamisamaDirector::RemoveExited()
{
    // Actor が消えるのは次のフレームの頭。ポインタはここで手放す
    std::erase_if(kamisamas_, [](Kamisama* kamisama)
                  {
                      if (!kamisama->IsExited())
                      {
                          return false;
                      }
                      kamisama->GetOwner()->Destroy();
                      return true;
                  });
}

void KamisamaDirector::AttachToLane(Kamisama* kamisama, Lane* lane)
{
    if (kamisama && lane)
    {
        lane->AttachKamisama(kamisama);
        kamisama->AttachToLane(lane->GetPosition(), lane->GetPlate());
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

bool KamisamaDirector::ShouldExit(const Kamisama& kamisama) const
{
    if(!kamisama.IsEating()) return false;

    return kamisama.IsSatisfied();
}

#ifdef TAKO_EDITOR
bool KamisamaDirector::DrawCustomImGui()
{
    ImGui::SeparatorText("Debug");

    // 編集中に出すと、かみさまがシーンに残って一緒に保存されるので、Play 中だけ押せる
    const bool playing = GetOwner()->GetScene()->IsPlaying();

    ImGui::BeginDisabled(!playing || !CanSpawn());
    if (ImGui::Button("Spawn Wandering"))
    {
        Spawn();// Spawn の中の Initialize で徘徊から始まる
    }
    ImGui::EndDisabled();

    ImGui::SameLine();
    ImGui::Text("%zu / %d", kamisamas_.size(), kMaxKamisamaCount);

    if (!playing)
    {
        ImGui::TextDisabled("(Play only)");
    }

    // 保存する値（TPROPERTY）は変えていない
    return false;
}
#endif
