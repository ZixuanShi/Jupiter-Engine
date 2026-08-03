// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Concepts;

import std;

export namespace jpt
{
    template<typename T>
    concept Numeric = std::is_arithmetic_v<T>;

    template<typename T>
    concept Floating = std::is_floating_point_v<T>;
}
