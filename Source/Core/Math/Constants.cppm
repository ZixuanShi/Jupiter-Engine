// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Constants;

import jpt.Concepts;
import std;

export namespace jpt
{
    template<Floating T> constexpr T kPi      = static_cast<T>(3.14159265358979323846);
    template<Floating T> constexpr T kTwoPi   = kPi<T> * static_cast<T>(2);
    template<Floating T> constexpr T kHalfPi  = kPi<T> / static_cast<T>(2);
    template<Floating T> constexpr T kEpsilon = static_cast<T>(1e-6);

    // Max rather than zero, because zero is a valid index and handle.
    template<Numeric T> constexpr T kInvalid = std::numeric_limits<T>::max();
}
