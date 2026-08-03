// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.MathTests;

import jpt.Assert;
import jpt.Logger;
import jpt.Math;
import jpt.Matrix44;
import jpt.TypeDefs;
import jpt.Vector3;
import jpt.Vector4;

namespace
{
    constexpr float32 kTolerance = 1e-4f;

    bool AreClose(const jpt::Vec3& a, const jpt::Vec3& b)
    {
        return jpt::AreValuesClose(a.x, b.x, kTolerance) &&
               jpt::AreValuesClose(a.y, b.y, kTolerance) &&
               jpt::AreValuesClose(a.z, b.z, kTolerance);
    }

    bool AreClose(const jpt::Mat44& a, const jpt::Mat44& b)
    {
        for (usize col = 0; col < 4; ++col)
        {
            for (usize row = 0; row < 4; ++row)
            {
                if (!jpt::AreValuesClose(a.m[col][row], b.m[col][row], kTolerance))
                {
                    return false;
                }
            }
        }
        return true;
    }
}

export namespace jpt
{
    /** Runs at startup outside Release. These cannot be static_asserts: Perspective and LookAt
        reach std::tan and std::sqrt, which are not constexpr before C++26. */
    void RunMathTests()
    {
        {
            const Vec3 p = Mat44::Translate(Vec3(1, 2, 3)) * Vec3(0, 0, 0);
            Debug::Assert(AreClose(p, Vec3(1, 2, 3)), "Translate: got ({}, {}, {})", p.x, p.y, p.z);
        }
        {
            const Vec3 p = Mat44::Scale(Vec3(2, 3, 4)) * Vec3(1, 1, 1);
            Debug::Assert(AreClose(p, Vec3(2, 3, 4)), "Scale: got ({}, {}, {})", p.x, p.y, p.z);
        }
        {
            const Mat44 m = Mat44::Translate(Vec3(1, 2, 3)) * Mat44::Scale(Vec3(2, 2, 2));
            Debug::Assert(Mat44::Transpose(Mat44::Transpose(m)) == m, "Transpose is not an involution");
        }
        {
            const Mat44 trs = Mat44::Translate(Vec3(1, 2, 3)) *
                              Mat44::RotateY(ToRadians(35.0f)) *
                              Mat44::Scale(Vec3(2, 2, 2));

            Debug::Assert(AreClose(Mat44::Inverse(trs) * trs, Mat44::Identity()),
                          "Inverse(TRS) * TRS is not identity");
        }
        {
            // The assertion legacy never had. Its Forward() was +Z while LookAt was
            // right-handed, so this came out (-1, 0, 0) and call sites negated to compensate.
            const Mat44 view = Mat44::LookAt(Vec3(0, 0, 5), Vec3(0, 0, 0));
            const Vec3 right(view.m[0].x, view.m[1].x, view.m[2].x);

            Debug::Assert(AreClose(right, Vec3::Right()),
                          "LookAt right axis is ({}, {}, {}), expected (1, 0, 0)", right.x, right.y, right.z);
        }
        {
            // Metal, Vulkan and D3D all clip against z in [0, 1]. Legacy's OpenGL form put the
            // near plane at -1, discarding half the frustum.
            constexpr float32 zNear = 0.1f;
            constexpr float32 zFar  = 100.0f;
            const Mat44 proj = Mat44::Perspective(ToRadians(60.0f), 16.0f / 9.0f, zNear, zFar);

            const Vec4 atNear = proj * Vec4(0, 0, -zNear, 1);
            const Vec4 atFar  = proj * Vec4(0, 0, -zFar,  1);

            const float32 nearNdc = atNear.z / atNear.w;
            const float32 farNdc  = atFar.z  / atFar.w;

            Debug::Assert(AreValuesClose(nearNdc, 0.0f, kTolerance), "Perspective near maps to {}, expected 0", nearNdc);
            Debug::Assert(AreValuesClose(farNdc,  1.0f, kTolerance), "Perspective far maps to {}, expected 1",  farNdc);
        }

        Debug::Log("Math tests passed.");
    }
}
