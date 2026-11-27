#pragma once

enum class MiconIrSide
{
    Left,
    Right,
};


struct MiconSensorEvent
{
    MiconIrSide side;
};