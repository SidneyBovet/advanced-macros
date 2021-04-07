#pragma once

#include "spdlog/spdlog.h"

namespace macro_player
{
    class Logging
    {
    public:
        static void setup_logging(spdlog::level::level_enum log_level);
    };
}