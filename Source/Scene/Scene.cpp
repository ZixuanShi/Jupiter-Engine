// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/AppClient.h"
#include "Applications/Window/Window.h"

module jpt.Scene;

import jpt.Camera;
import jpt.Constants;
import jpt.GestureRecognizer;
import jpt.Input;
import jpt.InputCodes;
import jpt.InputEvents;
import jpt.Quaternion;
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

    bool Scene::Init()
    {
        if (!m_camera.Init())
        {
            return false;
        }

        Input& input = GetApplication().GetInput();

        input.OnPan().Add([this](const PanEvent& event)
            {
                if (event.fingerCount == 1)      
                { 
                    Rotate(event.delta); 
                }
                else if (event.fingerCount == 3) 
                { 
                    Translate(event.delta); 
                }
            });

        input.OnMouseMove().Add([this](const MouseMoveEvent& event)
            {
                if (GetApplication().GetInput().IsMouseButtonDown(MouseButton::Left))
                {
                    Rotate(event.delta);
                }
                else if (GetApplication().GetInput().IsMouseButtonDown(MouseButton::Middle))
                {
                    Translate(event.delta);
                }
                else if (GetApplication().GetInput().IsMouseButtonDown(MouseButton::Right))
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

    void Scene::Rotate(const Vec2& deltaPixels)
    {
        const float32 height = static_cast<float32>(GetApplication().GetWindow().GetHeight());
        if (height < 1.0f)
        {
            return;
        }

        m_yaw   += deltaPixels.x / height * kTwoPi<float32>;
        m_pitch += deltaPixels.y / height * kTwoPi<float32>;
        m_pyramid.rotation = Quat::FromAxisAngle(m_camera.Right(), m_pitch) 
                           * Quat::FromAxisAngle(Vec3::Up(), m_yaw);
    }

    void Scene::Update()
    {
        Application& app = GetApplication();

        const Vec3 axis = GetMoveAxis(app.GetInput());
        if (axis == Vec3::Zero())
        {
            return;
        }

        // Per second, not per frame, so the speed does not follow the frame rate.
        constexpr float32 kSpeed = 4.0f;
        const float32 distance = static_cast<float32>(app.GetFrameTimer().GetDeltaSeconds()) * kSpeed;

        m_camera.MoveLocal(axis * distance);
    }

    void Scene::Look(const Vec2& deltaPixels)
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

        m_camera.RotateLocal(pitch, yaw);
    }

    void Scene::Translate(const Vec2& deltaPixels)
    {
        const float32 height = static_cast<float32>(GetApplication().GetWindow().GetHeight());
        Move(m_camera.ScreenDeltaToWorld(deltaPixels, height));
    }

    void Scene::Move(const Vec3& worldOffset)
    {
        constexpr float32 kRange = 3.0f;

        Vec3 position = m_pyramid.position + worldOffset;
        position.x = std::clamp(position.x, -kRange, kRange);
        position.y = std::clamp(position.y, -kRange, kRange);
        position.z = std::clamp(position.z, -kRange, kRange);

        m_pyramid.position = position;
    }
}
