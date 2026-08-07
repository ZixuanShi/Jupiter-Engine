// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.Transform;

import jpt.Matrix44;
import jpt.Quaternion;
import jpt.Vector3;

namespace jpt
{
    Mat44 Transform::ToMatrix() const noexcept
    {
        return Mat44::Translate(position) * rotation.ToMatrix() * Mat44::Scale(scale);
    }
}
