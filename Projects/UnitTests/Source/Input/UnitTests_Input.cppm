// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Input;

import jpt.TestFramework;

import UnitTests_Gesture;
import UnitTests_InputCodes;
import UnitTests_InputState;
import UnitTests_SdlEvent;

export void RunUnitTests_Input()
{
    jpt::TestCase::Run("Input.TouchMapping",            &RunUnitTests_TouchMapping);
    jpt::TestCase::Run("Input.NameTables",              &RunUnitTests_NameTables);
    jpt::TestCase::Run("Input.KeyAndMouse",             &RunUnitTests_KeyAndMouse);
    jpt::TestCase::Run("Input.Capture",                 &RunUnitTests_Capture);
    jpt::TestCase::Run("Input.GesturePan",              &RunUnitTests_GesturePan);
    jpt::TestCase::Run("Input.GestureDirectVsIndirect", &RunUnitTests_GestureDirectVsIndirect);
    jpt::TestCase::Run("Input.GestureCapture",          &RunUnitTests_GestureCapture);
    jpt::TestCase::Run("Input.GestureTwist",            &RunUnitTests_GestureTwist);

    // Last: the only case here that needs the live Window, rather than an Input of its own.
    jpt::TestCase::Run("Input.SdlEventTranslation", &RunUnitTests_SdlEventTranslation);
}
