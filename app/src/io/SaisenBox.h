#pragma once

class ISaisenInput
{
public:
    virtual ~ISaisenInput() = default;

    virtual float GetWeightRaw() const = 0;
    virtual float GetWeight() const = 0;
};
