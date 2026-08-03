// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.Camera;

import jpt.Matrix44;
import jpt.TypeDefs;
import jpt.Vector3;

namespace jpt
{
    void Camera::SetPosition(const Vec3& position) noexcept
    {
        m_position = position;
    }

    void Camera::SetTarget(const Vec3& target) noexcept
    {
        m_target = target;
    }

    Mat44 Camera::GetViewProjection(float32 aspect) const noexcept
    {
        Mat44 projection;

        if constexpr (kProjectionMode == ProjectionMode::Perspective)
        {
            projection = Mat44::Perspective(m_fovY, aspect, m_zNear, m_zFar);
        }
        else
        {
            projection = Mat44::Orthographic(m_orthoHeight * aspect, m_orthoHeight, m_zNear, m_zFar);
        }

        return projection * Mat44::LookAt(m_position, m_target);
    }
}
