// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Headers.h"

#include <variant>

export module UnitTests_Variant;

import jpt.Variant;
import jpt.Utilities;
import jpt.TypeDefs;

class LoggingTester
{
public:
    LoggingTester()
    {
        JPT_LOG("LogingTester::LogingTester()");
    }

    ~LoggingTester()
    {
        JPT_LOG("LogingTester::~LogingTester()");
    }

    LoggingTester(const LoggingTester&)
    {
        JPT_LOG("LogingTester::LogingTester(const LogingTester&)");
    }

    LoggingTester(LoggingTester&&)
    {
        JPT_LOG("LogingTester::LogingTester(LogingTester&&)");
    }

    LoggingTester& operator=(const LoggingTester&)
    {
        JPT_LOG("LogingTester::operator=(const LogingTester&)");
        return *this;
    }

    LoggingTester& operator=(LoggingTester&&)
    {
        JPT_LOG("LogingTester::operator=(LogingTester&&)");
        return *this;
    }
};

bool UnitTest_Variant()
{
    jpt::Variant<int32, char, bool, jpt::String, LoggingTester> variant;

    variant = 42;
    variant.As<int32>() += 2;
    JPT_LOG(variant.As<int32>());

    variant = jpt::String("Hello World");
    JPT_LOG(variant.As<jpt::String>());

    LoggingTester loggingTester;
    variant = LoggingTester();
    variant = loggingTester;

    return true;
}

bool UnitTest_stdVariant()
{
    std::variant<int32, char, bool, jpt::String> variant = "Hello World";

    variant = 5;
    variant = false;
    //JPT_LOG(std::get<bool>(variant));

    return true;
}

export bool RunUnitTests_Variant()
{
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_Variant(), "UnitTest_Variant Failed");
    JPT_RETURN_FALSE_IF_ERROR(!UnitTest_stdVariant(), "UnitTest_stdVariant Failed");

    return true;
}
