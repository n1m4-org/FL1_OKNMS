#pragma once

#include <cstdint>

class Score
{
public:
    Score() = default;
    ~Score() = default;
    Score(const uint32_t& v) : value_(v) {}

    Score& operator +=(uint32_t value);
    Score& operator -=(uint32_t value);
    Score operator +(uint32_t value) const;
    Score operator -(uint32_t value) const;
    Score& operator =(uint32_t value) = delete;

    uint32_t Get() const { return value_; }

private:
    uint32_t value_ = 0;
};
