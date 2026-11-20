#pragma once

#include <Script.h>
#include <type/Score.h>
#include <ActorRef.h>

TCLASS()
class ScoreManager : public Tako::Script
{
    GENERATED_BODY()

public:
#ifdef TAKO_EDITOR
    bool DrawCustomImGui() override;
#endif // TAKO_EDITOR

    void AddScore(uint32_t value);
    void SubtractScore(uint32_t value);

    Score GetScore() const { return score_; }

private:
    void ApplyToView();

    TPROPERTY(Edit, Required, Tooltip = "スコア表示用のActor")
    Tako::ActorRef viewActor_;
    Score score_ = 0;
};
