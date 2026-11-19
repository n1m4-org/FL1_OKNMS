#pragma once

#include <Script.h>
#include <type/Score.h>
#include <ActorRef.h>

TCLASS()
class ScoreManager : public Tako::Script
{
    GENERATED_BODY()

public:
#ifdef _DEBUG
    bool DrawCustomImGui() override;
#endif // _DEBUG

    void AddScore(uint32_t value);
    void SubtractScore(uint32_t value);

    Score GetScore() const { return score_; }

private:
    void ApplyToView();

    TPROPERTY(Edit, Required, ActorWith = "ScoreView", Tooltip = "スコア表示用のActor")
    Tako::ActorRef viewActor_;
    Score score_ = 0;
};
