// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests.Scene;

import UnitTests.TestFramework;

import UnitTests.Camera;

export void RunUnitTests_Scene()
{
    jpt::TestCase::Run("Scene.CameraScreenDelta", &RunUnitTests_CameraScreenDelta);
    jpt::TestCase::Run("Scene.CameraBasis",       &RunUnitTests_CameraBasis);
    jpt::TestCase::Run("Scene.CameraLocalMotion", &RunUnitTests_CameraLocalMotion);
    jpt::TestCase::Run("Scene.CameraMoveAxis",    &RunUnitTests_CameraMoveAxis);
}
