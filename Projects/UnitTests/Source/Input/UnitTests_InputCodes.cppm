// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_InputCodes;

import jpt.InputCodes;
import jpt.Math;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector2;
import std;

/** A 0..1 touch is normalized over the window for Direct and IndirectRelative, and over the
    pad's own extents for IndirectAbsolute -- so only the last takes the aspect correction.
    A 2:1 window makes the two answers differ. */
export void RunUnitTests_TouchMapping(jpt::TestCase& test)
{
    constexpr uint32 kWidth  = 1000;
    constexpr uint32 kHeight = 500;

    const jpt::Vec2 direct = jpt::ToTouchPixels(0.5f, 0.5f, jpt::TouchDevice::Direct, kWidth, kHeight);
    test.Expect(direct == jpt::Vec2(500.0f, 250.0f), "Direct mapped to ({}, {}), expected (500, 250)", direct.x, direct.y);

    // Window space, exactly as Direct -- SDL normalizes an indirect pointer over the window
    // too. This is the assertion that fails if the aspect leaks onto it.
    const jpt::Vec2 relative = jpt::ToTouchPixels(0.5f, 0.5f, jpt::TouchDevice::IndirectRelative, kWidth, kHeight);
    test.Expect(relative == direct, "IndirectRelative mapped to ({}, {}), expected the same as Direct ({}, {})", relative.x, relative.y, direct.x, direct.y);

    // Pad space: both axes take the height, and x carries the 1.6 pad aspect.
    const jpt::Vec2 absolute = jpt::ToTouchPixels(0.5f, 0.5f, jpt::TouchDevice::IndirectAbsolute, kWidth, kHeight);
    test.Expect(jpt::AreValuesClose(absolute.x, 400.0f), "IndirectAbsolute x is {}, expected 0.5 * 500 * 1.6 = 400", absolute.x);
    test.Expect(jpt::AreValuesClose(absolute.y, 250.0f), "IndirectAbsolute y is {}, expected 0.5 * 500 = 250", absolute.y);

    // The aspect must not touch y, or a twist reads skewed in the other direction.
    const jpt::Vec2 tall = jpt::ToTouchPixels(0.0f, 1.0f, jpt::TouchDevice::IndirectAbsolute, kWidth, kHeight);
    test.Expect(tall == jpt::Vec2(0.0f, 500.0f), "IndirectAbsolute scaled y by the aspect: ({}, {})", tall.x, tall.y);
}

/** Checked at both ends, because a short table only misbehaves at the end. */
export void RunUnitTests_NameTables(jpt::TestCase& test)
{
    test.Expect(std::string_view(jpt::ToString(jpt::KeyCode::A)) == "A", "KeyCode::A is named {}", jpt::ToString(jpt::KeyCode::A));
    test.Expect(std::string_view(jpt::ToString(jpt::KeyCode::KeypadClear)) == "KeypadClear", "The last KeyCode is named {}", jpt::ToString(jpt::KeyCode::KeypadClear));
    test.Expect(std::string_view(jpt::ToString(jpt::KeyCode::LeftSuper)) == "LeftSuper", "KeyCode::LeftSuper is named {}", jpt::ToString(jpt::KeyCode::LeftSuper));
    test.Expect(std::string_view(jpt::ToString(jpt::MouseButton::Middle)) == "Middle", "MouseButton::Middle is named {}", jpt::ToString(jpt::MouseButton::Middle));
}
