#include "Lane.h"

void Lane::Initialize(const Tako::Vector3& position, Plate* plate)
{
    position_ = position;
    pKamisama_ = nullptr;

    if(!plate) return;
    pPlate_ = plate;
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
