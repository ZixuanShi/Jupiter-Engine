// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.InputCodesTests;

import jpt.InputCodes;
import jpt.Math;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector2;
import std;

namespace jpt::local
{
    /** A 0..1 touch is normalized over the window for Direct and IndirectRelative, and over the
        pad's own extents for IndirectAbsolute -- so only the last takes the aspect correction.
        A 2:1 window makes the two answers differ. */
    void TouchMappingTest(TestCase& test)
    {
        constexpr uint32 kWidth  = 1000;
        constexpr uint32 kHeight = 500;

        const Vec2 direct = ToTouchPixels(0.5f, 0.5f, TouchDevice::Direct, kWidth, kHeight);
        test.Expect(direct == Vec2(500.0f, 250.0f), "Direct mapped to ({}, {}), expected (500, 250)", direct.x, direct.y);

        // Window space, exactly as Direct -- SDL normalizes an indirect pointer over the window
        // too. This is the assertion that fails if the aspect leaks onto it.
        const Vec2 relative = ToTouchPixels(0.5f, 0.5f, TouchDevice::IndirectRelative, kWidth, kHeight);
        test.Expect(relative == direct, "IndirectRelative mapped to ({}, {}), expected the same as Direct ({}, {})", relative.x, relative.y, direct.x, direct.y);

        // Pad space: both axes take the height, and x carries the 1.6 pad aspect.
        const Vec2 absolute = ToTouchPixels(0.5f, 0.5f, TouchDevice::IndirectAbsolute, kWidth, kHeight);
        test.Expect(AreValuesClose(absolute.x, 400.0f), "IndirectAbsolute x is {}, expected 0.5 * 500 * 1.6 = 400", absolute.x);
        test.Expect(AreValuesClose(absolute.y, 250.0f), "IndirectAbsolute y is {}, expected 0.5 * 500 = 250", absolute.y);

        // The aspect must not touch y, or a twist reads skewed in the other direction.
        const Vec2 tall = ToTouchPixels(0.0f, 1.0f, TouchDevice::IndirectAbsolute, kWidth, kHeight);
        test.Expect(tall == Vec2(0.0f, 500.0f), "IndirectAbsolute scaled y by the aspect: ({}, {})", tall.x, tall.y);
    }

    /** Checked at both ends, because a short table only misbehaves at the end. */
    void NameTableTest(TestCase& test)
    {
        test.Expect(std::string_view(ToString(KeyCode::A)) == "A", "KeyCode::A is named {}", ToString(KeyCode::A));
        test.Expect(std::string_view(ToString(KeyCode::KeypadClear)) == "KeypadClear", "The last KeyCode is named {}", ToString(KeyCode::KeypadClear));
        test.Expect(std::string_view(ToString(KeyCode::LeftSuper)) == "LeftSuper", "KeyCode::LeftSuper is named {}", ToString(KeyCode::LeftSuper));
        test.Expect(std::string_view(ToString(MouseButton::Middle)) == "Middle", "MouseButton::Middle is named {}", ToString(MouseButton::Middle));
    }

    static TestCase s_touchMapping("Input.TouchMapping", &TouchMappingTest);
    static TestCase s_nameTable("Input.NameTables", &NameTableTest);
}
