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

        // The desktop equivalent of a one-finger drag. Reuses the polled button state rather
        // than tracking its own.
        input.OnMouseMove().Add([this](const MouseMoveEvent& event)
            {
                if (GetApplication().GetInput().IsMouseButtonDown(MouseButton::Left))
                {
                    Rotate(event.delta);
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

        // Relative to the viewport, not per pixel, so the feel does not change with resolution.
        // A full-height drag is one turn.
        const float32 yaw   = deltaPixels.x / height * kTwoPi<float32>;
        const float32 pitch = deltaPixels.y / height * kTwoPi<float32>;

        // World axes, applied outside the existing rotation, so dragging always moves the near
        // face the way the finger went regardless of how far it has already been turned.
        m_pyramid.rotation = Quat::FromAxisAngle(Vec3::Up(), yaw)
                           * Quat::FromAxisAngle(Vec3::Right(), pitch)
                           * m_pyramid.rotation;
    }

    void Scene::Translate(const Vec2& deltaPixels)
    {
        constexpr float32 kRange = 3.0f;

        const float32 height = static_cast<float32>(GetApplication().GetWindow().GetHeight());

        Vec3 position = m_pyramid.position + m_camera.ScreenDeltaToWorld(deltaPixels, height);
        position.x = std::clamp(position.x, -kRange, kRange);
        position.y = std::clamp(position.y, -kRange, kRange);
        position.z = std::clamp(position.z, -kRange, kRange);

        m_pyramid.position = position;
    }
}
