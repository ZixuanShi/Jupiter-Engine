// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/AppClient.h"

module jpt.Camera;

import jpt.Constants;
import jpt.Input;
import jpt.InputEvents;
import jpt.GestureRecognizer;
import jpt.Matrix44;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;
import std;

namespace jpt
{
    bool Camera::Init()
    {
        Input& input = GetApplication().GetInput();

        input.OnPinch().Add([this](const PinchEvent& event)
            {
                // Fingers separating means a larger scale, which means moving closer.
                Zoom(1.0f / event.scale);
            });

        input.OnMouseScroll().Add([this](const MouseScrollEvent& event)
            {
                // exp keeps the factor positive and makes the zoom multiplicative, so equal
                // scroll in each direction lands back where it started.
                const float32 rate = event.isPrecise ? 0.01f : 0.1f;
                Zoom(std::exp(-event.delta.y * rate));
            });

        return true;
    }

    void Camera::Zoom(float32 factor) noexcept
    {
        constexpr float32 kMinDistance = 0.5f;
        constexpr float32 kMaxDistance = 50.0f;

        const Vec3 toEye = m_position - m_target;
        const float32 distance = toEye.Length();
        if (distance < kEpsilon<float32>)
        {
            return;
        }

        // Apply the clamped factor to both, so a clamp does not desync orthoHeight from distance.
        const float32 applied = std::clamp(distance * factor, kMinDistance, kMaxDistance) / distance;
        m_position = m_target + toEye * applied;
        m_orthoHeight *= applied;
    }

    Vec3 Camera::ScreenDeltaToWorld(const Vec2& deltaPixels, float32 viewportHeight) const noexcept
    {
        const float32 distance = (m_target - m_position).Length();
        if (viewportHeight < 1.0f || distance < kEpsilon<float32>)
        {
            return Vec3::Zero();
        }

        // The rows of the view rotation are the camera basis in world space, and LookAt already
        // guards the degenerate top-down case rather than this repeating the guard.
        const Mat44 view = Mat44::LookAt(m_position, m_target);
        const Vec3 right(view.m[0].x, view.m[1].x, view.m[2].x);
        const Vec3 up   (view.m[0].y, view.m[1].y, view.m[2].y);

        const float32 worldPerPixel = (m_projectionMode == ProjectionMode::Perspective)
            ? (2.0f * distance * std::tan(m_fovY * 0.5f)) / viewportHeight
            : m_orthoHeight / viewportHeight;

        // Screen Y is down, world Y is up.
        return right * (deltaPixels.x * worldPerPixel) - up * (deltaPixels.y * worldPerPixel);
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
