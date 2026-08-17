// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.ProjectionTests;

import jpt.Constants;
import jpt.Math;
import jpt.MathTestUtils;
import jpt.Matrix44;
import jpt.TestFramework;
import jpt.TypeDefs;
import jpt.Vector4;
import std;

namespace jpt::local
{
    /** Right-handed with depth in [0, 1], which is what Metal, Vulkan and D3D clip against.
        Legacy emitted OpenGL's [-1, 1]. */
    void PerspectiveTest(TestCase& test)
    {
        constexpr float32 zNear  = 0.1f;
        constexpr float32 zFar   = 100.0f;
        constexpr float32 fovY   = kHalfPi<float32> / 1.5f;      // 60 degrees
        constexpr float32 aspect = 16.0f / 9.0f;

        const Mat44 proj = Mat44::Perspective(fovY, aspect, zNear, zFar);

        const Vec4 atNear = proj * Vec4(0, 0, -zNear, 1);
        const Vec4 atFar  = proj * Vec4(0, 0, -zFar,  1);

        test.Expect(AreValuesClose(atNear.z / atNear.w, 0.0f, kTestTolerance),
                    "Perspective near maps to {}, expected 0", atNear.z / atNear.w);
        test.Expect(AreValuesClose(atFar.z / atFar.w, 1.0f, kTestTolerance),
                    "Perspective far maps to {}, expected 1", atFar.z / atFar.w);

        // w must carry -z_view, which is what makes the divide a perspective divide.
        test.Expect(AreValuesClose(atFar.w, zFar), "Perspective w is not -z_view");

        // Top of the frustum maps to +1: Metal's NDC has +Y up, and no flip belongs here.
        const float32 halfHeight = std::tan(fovY * 0.5f) * 2.0f;
        const Vec4 atTop   = proj * Vec4(0.0f, halfHeight, -2.0f, 1.0f);
        const Vec4 atRight = proj * Vec4(halfHeight * aspect, 0.0f, -2.0f, 1.0f);

        test.Expect(AreValuesClose(atTop.y / atTop.w, 1.0f, kTestTolerance),
                    "Perspective top edge maps to {}, expected +1", atTop.y / atTop.w);
        test.Expect(AreValuesClose(atRight.x / atRight.w, 1.0f, kTestTolerance),
                    "Perspective right edge maps to {}, expected +1", atRight.x / atRight.w);

        // Depth must increase with distance, or the depth test compares backwards.
        const Vec4 atMid = proj * Vec4(0, 0, -10.0f, 1);
        test.Expect(atNear.z / atNear.w < atMid.z / atMid.w && atMid.z / atMid.w < atFar.z / atFar.w,
                    "Perspective depth is not monotonically increasing");
    }

    /** The same [0, 1] depth range as Perspective, but w stays 1, so nothing converges with
        distance. */
    void OrthographicTest(TestCase& test)
    {
        constexpr float32 zNear  = 0.1f;
        constexpr float32 zFar   = 100.0f;
        constexpr float32 height = 4.0f;
        constexpr float32 aspect = 16.0f / 9.0f;

        const Mat44 proj = Mat44::Orthographic(height * aspect, height, zNear, zFar);

        const Vec4 atNear = proj * Vec4(0, 0, -zNear, 1);
        const Vec4 atFar  = proj * Vec4(0, 0, -zFar,  1);

        test.Expect(AreValuesClose(atNear.z, 0.0f, kTestTolerance), "Orthographic near maps to {}, expected 0", atNear.z);
        test.Expect(AreValuesClose(atFar.z,  1.0f, kTestTolerance), "Orthographic far maps to {}, expected 1", atFar.z);

        // w untouched, so the perspective divide does nothing. This is the definition.
        test.Expect(AreValuesClose(atFar.w, 1.0f), "Orthographic w is {}, expected 1", atFar.w);

        // The box edges map to +1, and to the same place at any depth -- which is exactly what
        // Perspective would not do.
        const Vec4 near = proj * Vec4(height * aspect * 0.5f, height * 0.5f,  -1.0f, 1);
        const Vec4 far  = proj * Vec4(height * aspect * 0.5f, height * 0.5f, -50.0f, 1);

        test.Expect(AreValuesClose(near.x, 1.0f, kTestTolerance), "Orthographic right edge maps to {}, expected +1", near.x);
        test.Expect(AreValuesClose(near.y, 1.0f, kTestTolerance), "Orthographic top edge maps to {}, expected +1", near.y);
        test.Expect(AreValuesClose(near.x, far.x, kTestTolerance), "Orthographic x converges with depth");

        // Depth must still increase with distance, or the depth test compares backwards.
        test.Expect(atNear.z < far.z && far.z < atFar.z, "Orthographic depth is not monotonically increasing");
    }

    static TestCase s_perspective("Math.Perspective", &PerspectiveTest);
    static TestCase s_orthographic("Math.Orthographic", &OrthographicTest);
}
