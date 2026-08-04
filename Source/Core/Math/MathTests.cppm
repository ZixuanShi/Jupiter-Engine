// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.MathTests;

import jpt.Assert;
import jpt.Constants;
import jpt.Logger;
import jpt.Math;
import jpt.Matrix44;
import jpt.Quaternion;
import jpt.Transform;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;
import jpt.Vector4;
import std;

namespace
{
    // Looser than kEpsilon: these compare against hand-written expectations.
    constexpr float32 kTolerance = 1e-4f;

    template<typename T>
    [[nodiscard]] constexpr bool AreClose(const T& a, const T& b) noexcept
    {
        return jpt::AreValuesClose(a, b, kTolerance);
    }
}

export namespace jpt
{
    /** Runs at startup outside Release. These cannot be static_asserts: Perspective and LookAt
        reach std::tan and std::sqrt, which are not constexpr before C++26. */
    void RunMathTests()
    {
        // Vector2
        {
            Debug::Assert(AreValuesClose(Vec2::Right().Dot(Vec2::Up()), 0.0f), "Vector2 basis is not orthogonal");

            Debug::Assert(AreValuesClose(Vec2(3.0f, 4.0f).Length(), 5.0f), "Vector2 length 3-4-5 is wrong");
            Debug::Assert(AreClose(Vec2(3.0f, 4.0f).Normalized(), Vec2(0.6f, 0.8f)), "Vector2 normalized 3-4-5 is wrong");
            Debug::Assert(AreClose(Vec2(2.0f), Vec2(2.0f, 2.0f)), "Vector2 scalar ctor does not broadcast");

            // Legacy compared against an exact zero, so this divided by 1e-20 and returned inf.
            const Vec2 tiny = Vec2(1e-20f, 0.0f).Normalized();
            Debug::Assert(std::isfinite(tiny.x) && std::isfinite(tiny.y), "Normalizing a near-zero Vector2 produced inf");
        }

        // Vector3 -- the basis must be right-handed, x cross y = z
        {
            Debug::Assert(AreClose(Vec3::Right().Cross(Vec3::Up()), Vec3::Backward()), "Right x Up != Backward");
            Debug::Assert(AreClose(Vec3::Up().Cross(Vec3::Backward()), Vec3::Right()), "Up x Backward != Right");
            Debug::Assert(AreClose(Vec3::Backward().Cross(Vec3::Right()), Vec3::Up()), "Backward x Right != Up");

            // Anti-commutative, so a reversed pair flips sign.
            Debug::Assert(AreClose(Vec3::Up().Cross(Vec3::Right()), Vec3::Forward()), "Up x Right != Forward");

            Debug::Assert(AreClose(Vec3::Forward(), -Vec3::Backward()), "Forward != -Backward");

            // Holds for objects, not only cameras -- and why "+X right, +Y up, +Z forward" cannot
            // be adopted: facing +Z puts right at -X.
            Debug::Assert(AreClose(Vec3::Forward().Cross(Vec3::Up()), Vec3::Right()), "Forward x Up != Right");
            Debug::Assert(AreClose(Vec3::Backward().Cross(Vec3::Up()), Vec3::Left()), "Backward x Up != Left");
            Debug::Assert(AreValuesClose(Vec3::Right().Dot(Vec3::Up()), 0.0f), "Basis is not orthogonal");

            const Vec3 normalized = Vec3(3.0f, 0.0f, 4.0f).Normalized();
            Debug::Assert(AreClose(normalized, Vec3(0.6f, 0.0f, 0.8f)), "Normalized 3-4-5 is wrong");
            Debug::Assert(AreValuesClose(Vec3(3.0f, 0.0f, 4.0f).Length(), 5.0f), "Length 3-4-5 is wrong");
        }

        // Vector4
        {
            // Legacy dropped w here, which silently broke any 4-component dot product.
            Debug::Assert(AreValuesClose(Vec4(1, 2, 3, 4).Dot(Vec4(1, 1, 1, 1)), 10.0f), "Vector4::Dot ignores w");
            Debug::Assert(AreClose(Vec4(2.0f), Vec4(2, 2, 2, 2)), "Vector4 scalar ctor does not broadcast");
            Debug::Assert(AreClose(Vec4(Vec3(1, 2, 3), 4.0f).XYZ(), Vec3(1, 2, 3)), "Vector4::XYZ is wrong");
        }

        // Matrix44 -- transforms
        {
            const Vec3 translated = Mat44::Translate(Vec3(1, 2, 3)) * Vec3(0, 0, 0);
            Debug::Assert(AreClose(translated, Vec3(1, 2, 3)), "Translate is wrong");

            const Vec3 scaled = Mat44::Scale(Vec3(2, 3, 4)) * Vec3(1, 1, 1);
            Debug::Assert(AreClose(scaled, Vec3(2, 3, 4)), "Scale is wrong");

            Debug::Assert(AreClose(Mat44::Translate(Vec3(1, 2, 3)).GetTranslation(), Vec3(1, 2, 3)),
                          "GetTranslation reads the wrong column");

            const Mat44 m = Mat44::Translate(Vec3(1, 2, 3)) * Mat44::Scale(Vec3(2, 2, 2));
            Debug::Assert(Mat44::Transpose(Mat44::Transpose(m)) == m, "Transpose is not an involution");
            Debug::Assert(AreClose(Mat44::Inverse(m) * m, Mat44::Identity()), "Inverse(M) * M is not identity");
        }

        // Matrix44 -- right-handed rotations. A positive angle about an axis carries the
        // next basis vector in the cyclic order X -> Y -> Z(backward) -> X.
        {
            constexpr float32 quarter = kHalfPi<float32>;

            Debug::Assert(AreClose(Mat44::RotateX(quarter) * Vec3::Up(), Vec3::Backward()), "RotateX is not right-handed");
            Debug::Assert(AreClose(Mat44::RotateY(quarter) * Vec3::Backward(), Vec3::Right()), "RotateY is not right-handed");
            Debug::Assert(AreClose(Mat44::RotateZ(quarter) * Vec3::Right(), Vec3::Up()), "RotateZ is not right-handed");

            // Rotating about an axis leaves that axis fixed.
            Debug::Assert(AreClose(Mat44::RotateX(quarter) * Vec3::Right(), Vec3::Right()), "RotateX moved the X axis");
            Debug::Assert(AreClose(Mat44::RotateY(quarter) * Vec3::Up(), Vec3::Up()), "RotateY moved the Y axis");

            // A full turn is the identity, and rotations preserve length.
            Debug::Assert(AreClose(Mat44::RotateY(kTwoPi<float32>), Mat44::Identity()), "RotateY(2pi) is not identity");
            Debug::Assert(AreValuesClose((Mat44::RotateZ(0.7f) * Vec3(1, 2, 3)).Length(),
                                          Vec3(1, 2, 3).Length()), "Rotation does not preserve length");

            // Composition order: X * Y * Z means Z is applied to the vector first.
            const Vec3 euler(0.3f, -0.5f, 0.8f);
            const Mat44 composed = Mat44::RotateX(euler.x) * Mat44::RotateY(euler.y) * Mat44::RotateZ(euler.z);
            Debug::Assert(AreClose(Mat44::FromEulerAngles(euler), composed), "FromEulerAngles order is wrong");

            const Mat44 a = Mat44::RotateX(0.4f);
            const Mat44 b = Mat44::Translate(Vec3(5, 0, 0));
            const Vec3 v(1, 2, 3);
            Debug::Assert(AreClose((a * b) * v, a * (b * v)), "Matrix product is not associative with M * v");
        }

        // Matrix44 -- LookAt. Right-handed, so view space looks down -Z.
        {
            const Vec3 eye(0, 0, 5);
            const Mat44 view = Mat44::LookAt(eye, Vec3(0, 0, 0));

            // The rotation rows are the camera basis expressed in world space.
            const Vec3 right  (view.m[0].x, view.m[1].x, view.m[2].x);
            const Vec3 up     (view.m[0].y, view.m[1].y, view.m[2].y);
            const Vec3 back   (view.m[0].z, view.m[1].z, view.m[2].z);

            // The assertion legacy never had: its Forward() was +Z while LookAt was
            // right-handed, so this came out (-1, 0, 0) and call sites negated to compensate.
            Debug::Assert(AreClose(right, Vec3::Right()), "LookAt right axis is ({}, {}, {})", right.x, right.y, right.z);
            Debug::Assert(AreClose(up, Vec3::Up()), "LookAt up axis is ({}, {}, {})", up.x, up.y, up.z);
            Debug::Assert(AreClose(back, Vec3::Backward()), "LookAt third row is not the backward axis");

            // The target lands in front of the camera, which is negative Z in view space.
            const Vec3 target = view * Vec3(0, 0, 0);
            Debug::Assert(AreClose(target, Vec3(0, 0, -5)), "LookAt puts the target at ({}, {}, {})", target.x, target.y, target.z);

            // A point beyond the eye lands behind it, so positive Z.
            Debug::Assert((view * Vec3(0, 0, 10)).z > 0.0f, "LookAt does not place points behind the camera at +Z");

            // The camera itself maps to the view-space origin.
            Debug::Assert(AreClose(view * eye, Vec3(0, 0, 0)), "LookAt does not map the eye to the origin");

            // Forward parallel to up: the basis loses rank without going NaN, and the translation
            // column stays correct, so only unit length catches it.
            const Mat44 topDown = Mat44::LookAt(Vec3(0, 3, 0), Vec3::Zero());

            const Vec3 downRight(topDown.m[0].x, topDown.m[1].x, topDown.m[2].x);
            const Vec3 downUp   (topDown.m[0].y, topDown.m[1].y, topDown.m[2].y);
            const Vec3 downBack (topDown.m[0].z, topDown.m[1].z, topDown.m[2].z);

            Debug::Assert(AreValuesClose(downRight.Length(), 1.0f), "LookAt straight down: right axis has length {}", downRight.Length());
            Debug::Assert(AreValuesClose(downUp.Length(), 1.0f), "LookAt straight down: up axis has length {}", downUp.Length());
            Debug::Assert(AreValuesClose(downBack.Length(), 1.0f), "LookAt straight down: back axis has length {}", downBack.Length());
            Debug::Assert(AreValuesClose(downRight.Dot(downUp), 0.0f), "LookAt straight down: basis is not orthogonal");
        }

        // Matrix44 -- Perspective. Right-handed with depth in [0, 1], which is what Metal,
        // Vulkan and D3D clip against. Legacy emitted OpenGL's [-1, 1].
        {
            constexpr float32 zNear  = 0.1f;
            constexpr float32 zFar   = 100.0f;
            constexpr float32 fovY   = kHalfPi<float32> / 1.5f;      // 60 degrees
            constexpr float32 aspect = 16.0f / 9.0f;

            const Mat44 proj = Mat44::Perspective(fovY, aspect, zNear, zFar);

            const Vec4 atNear = proj * Vec4(0, 0, -zNear, 1);
            const Vec4 atFar  = proj * Vec4(0, 0, -zFar,  1);

            Debug::Assert(AreValuesClose(atNear.z / atNear.w, 0.0f, kTolerance),
                          "Perspective near maps to {}, expected 0", atNear.z / atNear.w);
            Debug::Assert(AreValuesClose(atFar.z / atFar.w, 1.0f, kTolerance),
                          "Perspective far maps to {}, expected 1", atFar.z / atFar.w);

            // w must carry -z_view, which is what makes the divide a perspective divide.
            Debug::Assert(AreValuesClose(atFar.w, zFar), "Perspective w is not -z_view");

            // Top of the frustum maps to +1: Metal's NDC has +Y up, and no flip belongs here.
            const float32 halfHeight = std::tan(fovY * 0.5f) * 2.0f;
            const Vec4 atTop   = proj * Vec4(0.0f, halfHeight, -2.0f, 1.0f);
            const Vec4 atRight = proj * Vec4(halfHeight * aspect, 0.0f, -2.0f, 1.0f);

            Debug::Assert(AreValuesClose(atTop.y / atTop.w, 1.0f, kTolerance),
                          "Perspective top edge maps to {}, expected +1", atTop.y / atTop.w);
            Debug::Assert(AreValuesClose(atRight.x / atRight.w, 1.0f, kTolerance),
                          "Perspective right edge maps to {}, expected +1", atRight.x / atRight.w);

            // Depth must increase with distance, or the depth test compares backwards.
            const Vec4 atMid = proj * Vec4(0, 0, -10.0f, 1);
            Debug::Assert(atNear.z / atNear.w < atMid.z / atMid.w && atMid.z / atMid.w < atFar.z / atFar.w,
                          "Perspective depth is not monotonically increasing");
        }

        // Matrix44 -- Orthographic. The same [0, 1] depth range as Perspective, but w stays 1,
        // so nothing converges with distance.
        {
            constexpr float32 zNear  = 0.1f;
            constexpr float32 zFar   = 100.0f;
            constexpr float32 height = 4.0f;
            constexpr float32 aspect = 16.0f / 9.0f;

            const Mat44 proj = Mat44::Orthographic(height * aspect, height, zNear, zFar);

            const Vec4 atNear = proj * Vec4(0, 0, -zNear, 1);
            const Vec4 atFar  = proj * Vec4(0, 0, -zFar,  1);

            Debug::Assert(AreValuesClose(atNear.z, 0.0f, kTolerance), "Orthographic near maps to {}, expected 0", atNear.z);
            Debug::Assert(AreValuesClose(atFar.z,  1.0f, kTolerance), "Orthographic far maps to {}, expected 1", atFar.z);

            // w untouched, so the perspective divide does nothing. This is the definition.
            Debug::Assert(AreValuesClose(atFar.w, 1.0f), "Orthographic w is {}, expected 1", atFar.w);

            // The box edges map to +1, and to the same place at any depth -- which is exactly
            // what Perspective would not do.
            const Vec4 near = proj * Vec4(height * aspect * 0.5f, height * 0.5f,  -1.0f, 1);
            const Vec4 far  = proj * Vec4(height * aspect * 0.5f, height * 0.5f, -50.0f, 1);

            Debug::Assert(AreValuesClose(near.x, 1.0f, kTolerance), "Orthographic right edge maps to {}, expected +1", near.x);
            Debug::Assert(AreValuesClose(near.y, 1.0f, kTolerance), "Orthographic top edge maps to {}, expected +1", near.y);
            Debug::Assert(AreValuesClose(near.x, far.x, kTolerance), "Orthographic x converges with depth");

            // Depth must still increase with distance, or the depth test compares backwards.
            Debug::Assert(atNear.z < far.z && far.z < atFar.z, "Orthographic depth is not monotonically increasing");
        }

        // Quaternion -- must agree with Matrix44, which the sections above already pin.
        {
            Debug::Assert(AreClose(Quat::Identity().Right(),   Vec3::Right()),   "Quaternion identity right");
            Debug::Assert(AreClose(Quat::Identity().Up(),      Vec3::Up()),      "Quaternion identity up");
            Debug::Assert(AreClose(Quat::Identity().Forward(), Vec3::Forward()), "Quaternion identity forward");

            // Legacy's Right() and Up() carried the opposite w signs -- the transpose, which is
            // the inverse rotation -- and its Forward() was +Z. This is where that shows.
            const Quat yaw = Quat::FromAxisAngle(Vec3::Up(), kHalfPi<float32>);
            Debug::Assert(AreClose(yaw.Right(), Vec3::Forward()), "Quaternion right after 90 deg yaw");
            Debug::Assert(AreClose(yaw.ToMatrix(), Mat44::RotateY(kHalfPi<float32>)), "Quaternion does not match RotateY");

            // Off-axis on all three, so an XYZ-order mismatch between the two paths cannot cancel.
            const Vec3 euler(ToRadians(20.0f), ToRadians(35.0f), ToRadians(50.0f));
            Debug::Assert(AreClose(Quat::FromEulerAngles(euler).ToMatrix(), Mat44::FromEulerAngles(euler)),
                          "Quaternion and Matrix44 Euler conventions disagree");

            // A sign error in operator* stays invisible until two rotations are chained.
            const Quat a = Quat::FromAxisAngle(Vec3::Up(), ToRadians(40.0f));
            const Quat b = Quat::FromAxisAngle(Vec3::Right(), ToRadians(25.0f));
            Debug::Assert(AreClose((a * b).ToMatrix(), a.ToMatrix() * b.ToMatrix()), "Quaternion product order is wrong");

            Debug::Assert(AreClose(a * a.Inverse(), Quat::Identity()), "q * q.Inverse() is not identity");

            Debug::Assert(AreClose(Quat::Slerp(a, b, 0.0f), a), "Slerp at t=0 is not the start");
            Debug::Assert(AreClose(Quat::Slerp(a, b, 1.0f), b), "Slerp at t=1 is not the end");
            Debug::Assert(AreValuesClose(Quat::Slerp(a, b, 0.5f).Length(), 1.0f), "Slerp does not stay unit length");
        }

        // Transform
        {
            Debug::Assert(AreClose(Transform{}.ToMatrix(), Mat44::Identity()), "Transform identity");

            // Pins all three orderings at once: +X scales to (2,0,0), yaws to (0,0,-2),
            // translates to (0,0,3). Swapping any pair moves the result.
            const Transform transform{ .position = Vec3(0.0f, 0.0f, 5.0f),
                                       .rotation = Quat::FromAxisAngle(Vec3::Up(), kHalfPi<float32>),
                                       .scale    = Vec3(2.0f) };
            Debug::Assert(AreClose(transform.ToMatrix() * Vec3::Right(), Vec3(0.0f, 0.0f, 3.0f)),
                          "Transform does not compose scale, then rotation, then translation");
        }

        Debug::Log("Math tests passed.");
    }
}
