#include "Lane.h"

void Lane::Initialize(const Tako::Vector3& position)
{
    position_ = position;
    pKamisama_ = nullptr;
}

void Lane::AttachKamisama(Kamisama* kamisama)
{
    if (!kamisama)
        return;

    pKamisama_ = kamisama;
}

void Lane::DetachKamisama()
{
    pKamisama_ = nullptr;
}
