// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/AppClient.h"
#include "Applications/Window/Window.h"

module jpt.Scene;

import jpt.Camera;
import jpt.Constants;
import jpt.FrameTimer;
import jpt.GestureRecognizer;
import jpt.Input;
import jpt.InputCodes;
import jpt.InputEvents;
import jpt.Light;
import jpt.LinearColor;
import jpt.Material;
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

        // Lights
        m_pointLights[0] = PointLight{ Vec3( 2.0f, 2.5f,  2.0f), LinearColor(1.00f, 0.96f, 0.88f), 100.0f };
        m_pointLights[1] = PointLight{ Vec3(-2.5f, 0.8f,  1.5f), LinearColor(0.72f, 0.80f, 1.00f),  40.0f };
        m_pointLights[2] = PointLight{ Vec3(-1.0f, 1.5f, -2.5f), LinearColor(1.00f, 0.88f, 0.78f),  60.0f };

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
        input.OnTwist().Add([this](const TwistEvent& event)
            {
                TwistModel(event.radians);
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
        ApplyRotation();
    }

    void Scene::TwistModel(float32 radians)
    {
        m_roll += radians;
        ApplyRotation();
    }

    void Scene::ApplyRotation()
    {
        // Roll outermost: a twist spins what is already on screen, about the axis you look along,
        // so it applies after yaw and pitch have placed the model.
        m_model.rotation = Quat::FromAxisAngle(m_camera.Forward(), m_roll)
                           * Quat::FromAxisAngle(m_camera.Right(), m_pitch)
                           * Quat::FromAxisAngle(Vec3::Up(), m_yaw);
    }

    void Scene::Update()
    {
        m_camera.Update();

        const float32 deltaSeconds = GetApplication().GetFrameTimer().GetDeltaSeconds();
        const float32 step = m_material.dissolveSpeed * deltaSeconds;
        m_material.dissolvePct = std::clamp(m_material.dissolvePct + (m_material.dissolving ? step : -step), 0.0f, 1.0f);
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
