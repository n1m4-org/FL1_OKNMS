#include "Score.h"



Score Score::operator-(uint32_t value) const
{
    Score result = *this;
    result.value_ -= value;
    return result;
}

Score Score::operator+(uint32_t value) const
{
    Score result = *this;
    result += value;
    return result;
}

Score& Score::operator+=(uint32_t value)
{
    value_ += value;
    return *this;
}

Score& Score::operator-=(uint32_t value)
{
    value_ -= value;
    return *this;
}
