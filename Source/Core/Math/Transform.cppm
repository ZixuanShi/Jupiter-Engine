// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Transform;

import jpt.Matrix44;
import jpt.Quaternion;
import jpt.Vector3;

export namespace jpt
{
    struct Transform
    {
        Vec3 position = Vec3::Zero();
        Quat rotation = Quat::Identity();
        Vec3 scale    = Vec3::One();

        // Scale, then rotate, then translate.
        [[nodiscard]] Mat44 ToMatrix() const noexcept
        {
            return Mat44::Translate(position) * rotation.ToMatrix() * Mat44::Scale(scale);
        }
    };
}
