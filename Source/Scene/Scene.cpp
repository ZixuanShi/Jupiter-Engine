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
    Vec2 GetMoveAxis(const Input& input) noexcept
    {
        Vec2 axis = Vec2::Zero();

        if (input.IsKeyDown(KeyCode::D) || input.IsKeyDown(KeyCode::RightArrow))
        {
            axis.x += 1.0f;
        }
        if (input.IsKeyDown(KeyCode::A) || input.IsKeyDown(KeyCode::LeftArrow))
        {
            axis.x -= 1.0f;
        }
        if (input.IsKeyDown(KeyCode::W) || input.IsKeyDown(KeyCode::UpArrow))
        {
            axis.y += 1.0f;
        }
        if (input.IsKeyDown(KeyCode::S) || input.IsKeyDown(KeyCode::DownArrow))
        {
            axis.y -= 1.0f;
        }

        // Normalized, or holding two keys moves 1.41x as fast as holding one.
        if (axis != Vec2::Zero())
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
        m_pyramid.rotation = Quat::FromAxisAngle(m_camera.GetRight(), m_pitch) 
                           * Quat::FromAxisAngle(Vec3::Up(), m_yaw);
    }

    void Scene::Update()
    {
        Application& app = GetApplication();

        const Vec2 axis = GetMoveAxis(app.GetInput());
        if (axis == Vec2::Zero())
        {
            return;
        }

        // Per second, not per frame, so the speed does not follow the frame rate.
        constexpr float32 kSpeed = 2.0f;
        const float32 distance = static_cast<float32>(app.GetFrameTimer().GetDeltaSeconds()) * kSpeed;

        // The world XY plane, so a key always means the same direction no matter where the camera
        // is -- unlike the three-finger drag, which tracks the fingers and so must be camera-relative.
        Move(Vec3(axis.x * distance, axis.y * distance, 0.0f));
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
