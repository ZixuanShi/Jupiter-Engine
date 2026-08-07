// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/AppClient.h"
#include "Applications/Window/Window.h"

module jpt.Camera;

import jpt.Constants;
import jpt.FrameTimer;
import jpt.Input;
import jpt.InputCodes;
import jpt.InputEvents;
import jpt.GestureRecognizer;
import jpt.Matrix44;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;
import std;

namespace jpt
{
    Vec3 GetMoveAxis(const Input& input) noexcept
    {
        Vec3 axis = Vec3::Zero();

        if (input.IsKeyDown(KeyCode::D) || input.IsKeyDown(KeyCode::RightArrow))
        {
            axis.x += 1.0f;
        }
        if (input.IsKeyDown(KeyCode::A) || input.IsKeyDown(KeyCode::LeftArrow))
        {
            axis.x -= 1.0f;
        }
        if (input.IsKeyDown(KeyCode::E))
        {
            axis.y += 1.0f;
        }
        if (input.IsKeyDown(KeyCode::Q))
        {
            axis.y -= 1.0f;
        }
        if (input.IsKeyDown(KeyCode::S) || input.IsKeyDown(KeyCode::DownArrow))
        {
            axis.z += 1.0f;
        }
        if (input.IsKeyDown(KeyCode::W) || input.IsKeyDown(KeyCode::UpArrow))
        {
            axis.z -= 1.0f;
        }

        // Normalized, or holding two keys moves 1.41x as fast as holding one.
        if (axis != Vec3::Zero())
        {
            axis = axis.Normalized();
        }

        return axis;
    }

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

        input.OnMouseMove().Add([this](const MouseMoveEvent& event)
            {
                if (GetApplication().GetInput().IsMouseButtonDown(MouseButton::Right))
                {
                    Look(event.delta);
                }
            });

        // Driven by the transition, not polled: AppKit counts hide against unhide, so calling
        // either every frame would sink the counter and strand the pointer.
        input.OnMouseButton().Add([](const MouseButtonEvent& event)
            {
                if (event.button == MouseButton::Right)
                {
                    GetApplication().GetWindow().SetCursorCaptured(event.isDown);
                }
            });

        return true;
    }

    void Camera::Update()
    {
        const Application& app = GetApplication();

        const Vec3 axis = GetMoveAxis(app.GetInput());
        if (axis == Vec3::Zero())
        {
            return;
        }
        if (!app.GetInput().IsMouseButtonDown(MouseButton::Right))
        {
            return;
        }

        constexpr float32 kSpeed = 4.0f;
        const FrameTimer& timer = app.GetFrameTimer();
        MoveLocal(axis * timer.GetDeltaSeconds() * kSpeed);
    }

    void Camera::MoveLocal(const Vec3& offset) noexcept
    {
        m_position += Right() * offset.x + Up() * offset.y + Backward() * offset.z;
    }

    void Camera::RotateLocal(float32 pitchRadians, float32 yawRadians) noexcept
    {
        // Past vertical the view axis meets the yaw axis and heading stops being defined, so the
        // camera would flip over rather than keep tipping.
        constexpr float32 kMaxPitch = ToRadians(89.0f);

        const float32 pitch = std::asin(std::clamp(Forward().y, -1.0f, 1.0f));
        const float32 applied = std::clamp(pitch + pitchRadians, -kMaxPitch, kMaxPitch) - pitch;

        m_rotation = Quat::FromAxisAngle(Vec3::Up(), yawRadians)
                   * m_rotation
                   * Quat::FromAxisAngle(Vec3::Right(), applied);
        m_rotation.Normalize();
    }

    void Camera::Zoom(float32 factor) noexcept
    {
        constexpr float32 kMinDistance = 0.5f;
        constexpr float32 kMaxDistance = 50.0f;

        if (m_distance < kEpsilon<float32>)
        {
            return;
        }

        const float32 distance = std::clamp(m_distance * factor, kMinDistance, kMaxDistance);

        // Along the view axis, so what the camera orbits stays put and only the radius changes.
        // Apply the clamped factor to orthoHeight too, or a clamp desyncs it from the distance.
        const float32 applied = distance / m_distance;
        m_position += Forward() * (m_distance - distance);
        m_distance = distance;
        m_orthoHeight *= applied;
    }

    void Camera::LookAt(const Vec3& point) noexcept
    {
        const Vec3 toPoint = point - m_position;
        const float32 distance = toPoint.Length();
        if (distance < kEpsilon<float32>)
        {
            return;
        }

        SetDirection(toPoint);
        SetDistance(distance);
    }

    Vec3 Camera::ScreenDeltaToWorld(const Vec2& deltaPixels, float32 viewportHeight) const noexcept
    {
        const float32 distance = m_distance;
        if (viewportHeight < 1.0f || distance < kEpsilon<float32>)
        {
            return Vec3::Zero();
        }

        const float32 worldPerPixel = (m_projectionMode == ProjectionMode::Perspective)
                                      ? (2.0f * distance * std::tan(m_fovY * 0.5f)) / viewportHeight
                                      : m_orthoHeight / viewportHeight;

        // Screen Y is down, world Y is up.
        return Right() * (deltaPixels.x * worldPerPixel) - Up() * (deltaPixels.y * worldPerPixel);
    }

    void Camera::SetDirection(const Vec3& direction) noexcept
    {
        // A dragged ImGui slider passes through zero on its way across, so a bad value is normal
        // input rather than a caller error: keep the last good aim instead of asserting.
        const float32 length = direction.Length();
        if (length < kEpsilon<float32>)
        {
            return;
        }

        const Vec3 aim = direction / length;
        const float32 pitch = std::asin(std::clamp(aim.y, -1.0f, 1.0f));
        const float32 yaw   = std::atan2(-aim.x, -aim.z);

        m_rotation = Quat::FromAxisAngle(Vec3::Up(), yaw) * Quat::FromAxisAngle(Vec3::Right(), pitch);
    }

    void Camera::SetDistance(float32 distance) noexcept
    {
        m_distance = std::max(distance, kEpsilon<float32>);
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

        const Mat44 view = m_rotation.Conjugate().ToMatrix() * Mat44::Translate(-m_position);
        return projection * view;
    }

    void Camera::Look(const Vec2& deltaPixels)
    {
        const float32 height = static_cast<float32>(GetApplication().GetWindow().GetHeight());
        if (height < 1.0f)
        {
            return;
        }

        // Both negated: dragging right turns right, which is a negative yaw about world up, and
        // screen Y is down while a positive pitch looks up. A full-height drag is a half turn.
        const float32 yaw   = -deltaPixels.x / height * kPi<float32>;
        const float32 pitch = -deltaPixels.y / height * kPi<float32>;

        RotateLocal(pitch, yaw);
    }
}
