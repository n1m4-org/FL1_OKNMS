#pragma once

enum class MiconIrSide
{
    Left,
    Right,
};

namespace Event
{
    struct MiconSensorEvent
    {
        MiconIrSide side;
    };
};