#pragma once

#include <optional>
#include <string_view>

#include "MiconMessage.h"


class MiconParser
{
public:

    MiconParser() = default;
    ~MiconParser() = default;

   static std::optional<Event::MiconSensorEvent> Parse(std::string_view line);

private:


};