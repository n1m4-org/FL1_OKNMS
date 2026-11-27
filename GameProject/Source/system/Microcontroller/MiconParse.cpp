#include "MiconParse.h"

// MiconParser.cpp
namespace
{
    constexpr std::string_view kIrLeft  = "IRL";   // 左の赤外線センサーが反応
    constexpr std::string_view kIrRight = "IRR";   // 右の赤外線センサーが反応
}

std::optional<MiconSensorEvent> MiconParser::Parse(std::string_view line)
{
    if (line == kIrLeft)
    {
        return MiconSensorEvent{ MiconIrSide::Left };
    }
    else if (line == kIrRight)
    {
        return MiconSensorEvent{ MiconIrSide::Right };
    }

    return std::nullopt;
}
