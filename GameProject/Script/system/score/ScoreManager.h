#pragma once

#include <Script.h>
#include <score/Score.h>

TCLASS()
class ScoreManager : public Tako::Script
{
    GENERATED_BODY()

public:

    Score GetScore() const { return score_; }

private:
    Score score_ = 0;
};
