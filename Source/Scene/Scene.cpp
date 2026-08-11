// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Applications/GetApp.h"

module jpt.Scene;

import jpt.Window;
import jpt.Camera;
import jpt.Constants;
import jpt.FrameTimer;
import jpt.GestureRecognizer;
import jpt.Input;
import jpt.InputCodes;
import jpt.InputEvents;
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

        // Input callbacks
        Input& input = GetApp().GetInput();
        input.OnPan().Add(this, &Scene::OnPan);
        input.OnTwist().Add(this, &Scene::OnTwist);
        input.OnMouseMove().Add(this, &Scene::OnMouseMove);

        return true;
    }

    void Scene::Update()
    {
        m_camera.Update();

        const float32 deltaSeconds = GetApp().GetFrameTimer().GetDeltaSeconds();
        const float32 step = m_material.dissolveSpeed * deltaSeconds;
        m_material.dissolvePct = std::clamp(m_material.dissolvePct + (m_material.dissolving ? step : -step), 0.0f, 1.0f);
    }

    void Scene::RotateModel(const Vec2& deltaPixels)
    {
        const float32 height = static_cast<float32>(GetApp().GetWindow().GetHeight());
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

    void Scene::TranslateModel(const Vec2& deltaPixels)
    {
        constexpr float32 kRange = 3.0f;

        const float32 height = static_cast<float32>(GetApp().GetWindow().GetHeight());

        Vec3 position = m_model.position + m_camera.ScreenDeltaToWorld(deltaPixels, height);
        position.x = std::clamp(position.x, -kRange, kRange);
        position.y = std::clamp(position.y, -kRange, kRange);
        position.z = std::clamp(position.z, -kRange, kRange);

        m_model.position = position;
    }

    void Scene::ApplyRotation()
    {
        // Roll outermost: a twist spins what is already on screen, about the axis you look along,
        // so it applies after yaw and pitch have placed the model.
        m_model.rotation = Quat::FromAxisAngle(m_camera.Forward(), m_roll)
                           * Quat::FromAxisAngle(m_camera.Right(), m_pitch)
                           * Quat::FromAxisAngle(Vec3::Up(), m_yaw);
    }

    void Scene::OnPan(const PanEvent& event)
    {
        if (event.fingerCount == 1)
        {
            RotateModel(event.delta);
        }
        else if (event.fingerCount == 3)
        {
            TranslateModel(event.delta);
        }
    }

    void Scene::OnTwist(const TwistEvent& event)
    {
        TwistModel(event.radians);
    }

    void Scene::OnMouseMove(const MouseMoveEvent& event)
    {
        if (GetApp().GetInput().IsMouseButtonDown(MouseButton::Left))
        {
            RotateModel(event.delta);
        }
        else if (GetApp().GetInput().IsMouseButtonDown(MouseButton::Middle))
        {
            TranslateModel(event.delta);
        }
    }
}
