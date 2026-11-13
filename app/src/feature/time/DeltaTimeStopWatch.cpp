#include "DeltaTimeStopWatch.h"
#include <time/FrameTimer.h>


void DeltaTimeStopWatch::Start()
{
    isStart_ = true;
    isRunning_ = true;
}

void DeltaTimeStopWatch::Update()
{
    if (!isStart_ || !isRunning_) return;
    now_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();
}

void DeltaTimeStopWatch::Stop()
{
    isRunning_ = false;
}

void DeltaTimeStopWatch::Reset()
{
    isRunning_ = false;
    isStart_ = false;
    now_ = 0.0;
}
