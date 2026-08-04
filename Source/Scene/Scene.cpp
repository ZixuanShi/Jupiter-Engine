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
        // Camera
        m_camera.SetPosition(Vec3(1.09f, 1.36f, 1.64f));
        m_camera.LookAt(m_model.position);
        if (!m_camera.Init())
        {
            return false;
        }

        // Input callbacks
        Input& input = GetApplication().GetInput();
        input.OnPan().Add([this](const PanEvent& event)
            {
                if (event.fingerCount == 1)      
                { 
                    RotateModel(event.delta); 
                }
                else if (event.fingerCount == 3) 
                { 
                    TranslateModel(event.delta); 
                }
            });
        input.OnMouseMove().Add([this](const MouseMoveEvent& event)
            {
                if (GetApplication().GetInput().IsMouseButtonDown(MouseButton::Left))
                {
                    RotateModel(event.delta);
                }
                else if (GetApplication().GetInput().IsMouseButtonDown(MouseButton::Middle))
                {
                    TranslateModel(event.delta);
                }
            });

        return true;
    }

    void Scene::RotateModel(const Vec2& deltaPixels)
    {
        const float32 height = static_cast<float32>(GetApplication().GetWindow().GetHeight());
        if (height < 1.0f)
        {
            return;
        }

        m_yaw   += deltaPixels.x / height * kTwoPi<float32>;
        m_pitch += deltaPixels.y / height * kTwoPi<float32>;
        m_model.rotation = Quat::FromAxisAngle(m_camera.Right(), m_pitch) 
                           * Quat::FromAxisAngle(Vec3::Up(), m_yaw);
    }

    void Scene::Update()
    {
        m_camera.Update();
    }

    void Scene::TranslateModel(const Vec2& deltaPixels)
    {
        constexpr float32 kRange = 3.0f;

        const float32 height = static_cast<float32>(GetApplication().GetWindow().GetHeight());

        Vec3 position = m_model.position + m_camera.ScreenDeltaToWorld(deltaPixels, height);
        position.x = std::clamp(position.x, -kRange, kRange);
        position.y = std::clamp(position.y, -kRange, kRange);
        position.z = std::clamp(position.z, -kRange, kRange);

        m_model.position = position;
    }
}
