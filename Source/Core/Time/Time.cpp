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
}
