// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Core;

import jpt.TestFramework;

import UnitTests_EventDispatcher;

// Math
import UnitTests_LookAt;
import UnitTests_Matrix44;
import UnitTests_Projection;
import UnitTests_Quaternion;
import UnitTests_Transform;
import UnitTests_Vector2;
import UnitTests_Vector3;
import UnitTests_Vector4;

export void RunUnitTests_Core()
{
    jpt::TestCase::Run("Core.EventDispatcherRegistration",  &RunUnitTests_EventDispatcherRegistration);
    jpt::TestCase::Run("Core.EventDispatcherMemberBinding", &RunUnitTests_EventDispatcherMemberBinding);
    jpt::TestCase::Run("Core.EventDispatcherReentrancy",    &RunUnitTests_EventDispatcherReentrancy);

    // Math, in dependency order: the vectors before the matrix that composes them, and the
    // quaternion after, since it is checked against Matrix44's rotations.
    jpt::TestCase::Run("Math.Vector2",           &RunUnitTests_Vector2);
    jpt::TestCase::Run("Math.Vector3",           &RunUnitTests_Vector3);
    jpt::TestCase::Run("Math.Vector4",           &RunUnitTests_Vector4);
    jpt::TestCase::Run("Math.Matrix44Transform", &RunUnitTests_Matrix44Transform);
    jpt::TestCase::Run("Math.Matrix44Rotation",  &RunUnitTests_Matrix44Rotation);
    jpt::TestCase::Run("Math.Quaternion",        &RunUnitTests_Quaternion);
    jpt::TestCase::Run("Math.Transform",         &RunUnitTests_Transform);
    jpt::TestCase::Run("Math.LookAt",            &RunUnitTests_LookAt);
    jpt::TestCase::Run("Math.Perspective",       &RunUnitTests_Perspective);
    jpt::TestCase::Run("Math.Orthographic",      &RunUnitTests_Orthographic);
}
