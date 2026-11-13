#pragma once

#include <Vector3.h>

class Kamisama;

class Lane
{
public:
    Lane() = default;
    ~Lane() = default;

    void Initialize(const Tako::Vector3& position);

    const Tako::Vector3& GetPosition() const { return position_; }

    void AttachKamisama(Kamisama* kamisama);

    void DetachKamisama();

    bool IsAttached() const { return pKamisama_ != nullptr; }

    bool IsAttachedTo(Kamisama* kamisama) const { return pKamisama_ == kamisama; }
private:

    Tako::Vector3 position_;

    Kamisama* pKamisama_ = nullptr;
};
