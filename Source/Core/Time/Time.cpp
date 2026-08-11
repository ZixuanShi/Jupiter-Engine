// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.Time;

namespace jpt
{
    std::string Timestamp()
    {
        const std::time_t now = std::time(nullptr);
        char text[32] = {};
        std::strftime(text, sizeof(text), "%Y%m%d-%H%M%S", std::localtime(&now));
        return text;
    }

    float64 NanoSecsToSeconds(uint64 nanoseconds) noexcept
    {
        constexpr float64 kNanoSecsPerSecond = 1'000'000'000.0;
        return static_cast<float64>(nanoseconds) / kNanoSecsPerSecond;
    }
}
