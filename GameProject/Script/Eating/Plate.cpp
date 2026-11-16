#include "Plate.h"

#include <algorithm>

float Plate::Take(float amount) 
{
    if (amount <= 0.f) return 0.f;

    const float taken = std::min(amount, amount_);
    amount_ -= taken;

    return taken;
}
