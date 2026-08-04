// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.Camera;

import jpt.Matrix44;
import jpt.TypeDefs;
import jpt.Vector3;

namespace jpt
{
    bool Camera::PreInit() noexcept
    {
        m_position = Vec3(1.6f, 2.0f, 2.4f);
        m_target = Vec3::Zero();
        return true;
    }

    void Camera::SetPosition(const Vec3& position) noexcept
    {
        m_position = position;
    }

    void Camera::SetTarget(const Vec3& target) noexcept
    {
        m_target = target;
    }

    void Camera::SetProjectionMode(ProjectionMode mode) noexcept
    {
        m_projectionMode = mode;
    }

    void Camera::SetFovY(float32 radians) noexcept
    {
        m_fovY = radians;
    }

    void Camera::SetOrthoHeight(float32 worldUnits) noexcept
    {
        m_orthoHeight = worldUnits;
    }

    void Camera::SetNearFar(float32 zNear, float32 zFar) noexcept
    {
        m_zNear = zNear;
        m_zFar  = zFar;
    }

    Mat44 Camera::GetViewProjection(float32 aspect) const noexcept
    {
        Mat44 projection;

        if (m_projectionMode == ProjectionMode::Perspective)
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
