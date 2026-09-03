// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests_Projection;

import jpt.Constants;
import jpt.Math;
import jpt.MathTestUtils;
import jpt.Matrix44;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector4;
import std;

/** Right-handed with depth in [0, 1], which is what Metal, Vulkan and D3D clip against.
    Legacy emitted OpenGL's [-1, 1]. */
export void RunUnitTests_Perspective(jpt::TestCase& test)
{
    constexpr float32 zNear  = 0.1f;
    constexpr float32 zFar   = 100.0f;
    constexpr float32 fovY   = jpt::kHalfPi<float32> / 1.5f;      // 60 degrees
    constexpr float32 aspect = 16.0f / 9.0f;

    const jpt::Mat44 proj = jpt::Mat44::Perspective(fovY, aspect, zNear, zFar);

    const jpt::Vec4 atNear = proj * jpt::Vec4(0, 0, -zNear, 1);
    const jpt::Vec4 atFar  = proj * jpt::Vec4(0, 0, -zFar,  1);

    test.Expect(jpt::AreValuesClose(atNear.z / atNear.w, 0.0f, jpt::kTestTolerance),
                "Perspective near maps to {}, expected 0", atNear.z / atNear.w);
    test.Expect(jpt::AreValuesClose(atFar.z / atFar.w, 1.0f, jpt::kTestTolerance),
                "Perspective far maps to {}, expected 1", atFar.z / atFar.w);

    // w must carry -z_view, which is what makes the divide a perspective divide.
    test.Expect(jpt::AreValuesClose(atFar.w, zFar), "Perspective w is not -z_view");

    // Top of the frustum maps to +1: Metal's NDC has +Y up, and no flip belongs here.
    const float32 halfHeight = std::tan(fovY * 0.5f) * 2.0f;
    const jpt::Vec4 atTop   = proj * jpt::Vec4(0.0f, halfHeight, -2.0f, 1.0f);
    const jpt::Vec4 atRight = proj * jpt::Vec4(halfHeight * aspect, 0.0f, -2.0f, 1.0f);

    test.Expect(jpt::AreValuesClose(atTop.y / atTop.w, 1.0f, jpt::kTestTolerance),
                "Perspective top edge maps to {}, expected +1", atTop.y / atTop.w);
    test.Expect(jpt::AreValuesClose(atRight.x / atRight.w, 1.0f, jpt::kTestTolerance),
                "Perspective right edge maps to {}, expected +1", atRight.x / atRight.w);

    // Depth must increase with distance, or the depth test compares backwards.
    const jpt::Vec4 atMid = proj * jpt::Vec4(0, 0, -10.0f, 1);
    test.Expect(atNear.z / atNear.w < atMid.z / atMid.w && atMid.z / atMid.w < atFar.z / atFar.w,
                "Perspective depth is not monotonically increasing");
}

/** The same [0, 1] depth range as Perspective, but w stays 1, so nothing converges with
    distance. */
export void RunUnitTests_Orthographic(jpt::TestCase& test)
{
    constexpr float32 zNear  = 0.1f;
    constexpr float32 zFar   = 100.0f;
    constexpr float32 height = 4.0f;
    constexpr float32 aspect = 16.0f / 9.0f;

    const jpt::Mat44 proj = jpt::Mat44::Orthographic(height * aspect, height, zNear, zFar);

    const jpt::Vec4 atNear = proj * jpt::Vec4(0, 0, -zNear, 1);
    const jpt::Vec4 atFar  = proj * jpt::Vec4(0, 0, -zFar,  1);

    test.Expect(jpt::AreValuesClose(atNear.z, 0.0f, jpt::kTestTolerance), "Orthographic near maps to {}, expected 0", atNear.z);
    test.Expect(jpt::AreValuesClose(atFar.z,  1.0f, jpt::kTestTolerance), "Orthographic far maps to {}, expected 1", atFar.z);

    // w untouched, so the perspective divide does nothing. This is the definition.
    test.Expect(jpt::AreValuesClose(atFar.w, 1.0f), "Orthographic w is {}, expected 1", atFar.w);

    // The box edges map to +1, and to the same place at any depth -- which is exactly what
    // Perspective would not do.
    const jpt::Vec4 near = proj * jpt::Vec4(height * aspect * 0.5f, height * 0.5f,  -1.0f, 1);
    const jpt::Vec4 far  = proj * jpt::Vec4(height * aspect * 0.5f, height * 0.5f, -50.0f, 1);

    test.Expect(jpt::AreValuesClose(near.x, 1.0f, jpt::kTestTolerance), "Orthographic right edge maps to {}, expected +1", near.x);
    test.Expect(jpt::AreValuesClose(near.y, 1.0f, jpt::kTestTolerance), "Orthographic top edge maps to {}, expected +1", near.y);
    test.Expect(jpt::AreValuesClose(near.x, far.x, jpt::kTestTolerance), "Orthographic x converges with depth");

    // Depth must still increase with distance, or the depth test compares backwards.
    test.Expect(atNear.z < far.z && far.z < atFar.z, "Orthographic depth is not monotonically increasing");
}
