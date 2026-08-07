// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// Textual before the imports. AppClient.h here would close a cycle in EditorUI.cppm, but an
// implementation unit may reach the application.
#include "imgui.h"
#include "Applications/AppClient.h"
#include "Applications/Window/Window.h"
#include "Graphics/Renderer.h"

module jpt.EditorUI;

import jpt.Camera;
import jpt.FrameTimer;
import jpt.LinearColor;
import jpt.Material;
import jpt.Math;
import jpt.Scene;
import jpt.TypeDefs;
import jpt.Vector3;
import std;

namespace jpt
{
    void EditorUI::Draw()
    {
        const float fontSize = ImGui::GetFontSize();
        ImGui::SetNextWindowPos(ImVec2(fontSize, fontSize), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(fontSize * 26.0f, 0.0f), ImGuiCond_FirstUseEver);   // 0 height: fit to content.

        if (ImGui::Begin("Dev Menu"))
        {
            DrawPerformance();
            DrawRendering();
            DrawVfx();
            DrawCamera();
        }

        ImGui::End();

        DrawOverlay();
    }

    void EditorUI::DrawPerformance()
    {
        if (!ImGui::CollapsingHeader("Performance"))
        {
            return;
        }

        Renderer& renderer = GetApplication().GetRenderer();

        ImGui::Checkbox("Show Metrics", &m_showMetrics);

#if IS_PLATFORM_MACOS
        bool vsync = renderer.IsVSyncEnabled();
        if (ImGui::Checkbox("VSync", &vsync))
        {
            renderer.SetVSync(vsync);
        }
#endif

        if (ImGui::Button("Capture GPU Frame"))
        {
            renderer.RequestCapture();
        }

        ImGui::SameLine();
        if (ImGui::Button("Delete Traces"))   // Pull first on iOS: Scripts/Debug/pull_captures.py
        {
            renderer.DeleteCaptures();
        }
    }

    void EditorUI::DrawRendering()
    {
        if (!ImGui::CollapsingHeader("Rendering"))
        {
            return;
        }

        Material& material = GetApplication().GetScene().GetMaterial();

        ImGui::ColorEdit3("Base Color", &material.baseColor.r, ImGuiColorEditFlags_Float);
    }

    void EditorUI::DrawVfx()
    {
        if (!ImGui::CollapsingHeader("VFX"))
        {
            return;
        }

        Material& material = GetApplication().GetScene().GetMaterial();

        if (ImGui::Button("Play"))
        {
            material.dissolving = !material.dissolving;
        }

        ImGui::SliderFloat("Speed", &material.dissolveSpeed, 0.1f, 5.0f, "%.1fx");
        ImGui::SliderFloat("Edge Width", &material.dissolveEdge, 0.005f, 0.2f, "%.3f");
        ImGui::ColorEdit3("Color", &material.dissolveColor.r, ImGuiColorEditFlags_Float);
    }

    void EditorUI::DrawCamera()
    {
        if (!ImGui::CollapsingHeader("Camera"))
        {
            return;
        }

        Camera& camera = GetApplication().GetScene().GetCamera();

        const Vec3& position = camera.GetPosition();
        const Vec3 forward = camera.Forward();
        ImGui::Text("At   %6.2f %6.2f %6.2f", position.x, position.y, position.z);
        ImGui::Text("Look %6.2f %6.2f %6.2f", forward.x, forward.y, forward.z);

        Vec3 move = Vec3::Zero();
        if (ImGui::DragFloat3("Move", &move.x, 0.02f))
        {
            camera.MoveLocal(move);   // x right, y up, z backward.
        }

        float32 pitch = 0.0f;
        if (ImGui::DragFloat("Pitch", &pitch, 0.25f, 0.0f, 0.0f, "%.1f deg"))
        {
            camera.RotateLocal(ToRadians(pitch), 0.0f);
        }

        float32 yaw = 0.0f;
        if (ImGui::DragFloat("Yaw", &yaw, 0.25f, 0.0f, 0.0f, "%.1f deg"))
        {
            camera.RotateLocal(0.0f, ToRadians(yaw));
        }

        // The enum's order is the combo's order, so the cast is the index.
        const char* modes[] = { "Perspective", "Orthographic" };
        int32 mode = static_cast<int32>(camera.GetProjectionMode());
        if (ImGui::Combo("Projection", &mode, modes, IM_ARRAYSIZE(modes)))
        {
            camera.SetProjectionMode(static_cast<ProjectionMode>(mode));
        }

        // Only the control for the active mode: the other has no effect on the image, and a
        // slider that does nothing is worse than no slider.
        if (camera.GetProjectionMode() == ProjectionMode::Perspective)
        {
            float32 fovDegrees = ToDegrees(camera.GetFovY());
            if (ImGui::SliderFloat("FOV", &fovDegrees, 10.0f, 120.0f, "%.1f deg"))
            {
                camera.SetFovY(ToRadians(fovDegrees));
            }
        }
        else
        {
            float32 height = camera.GetOrthoHeight();
            if (ImGui::SliderFloat("Height", &height, 0.1f, 50.0f, "%.2f units"))
            {
                camera.SetOrthoHeight(height);
            }
        }

        // Logarithmic, because the useful range of a near plane spans four orders of
        // magnitude and depth precision is dominated by its low end.
        float32 zNear = camera.GetNear();
        float32 zFar  = camera.GetFar();

        bool changed = ImGui::DragFloat("Near", &zNear, 0.01f, 0.001f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
        changed     |= ImGui::DragFloat("Far",  &zFar,  1.0f,   0.1f, 10000.0f, "%.1f", ImGuiSliderFlags_Logarithmic);

        if (changed)
        {
            constexpr float32 kMinRange = 0.001f;
            camera.SetNearFar(zNear, std::max(zFar, zNear + kMinRange));
        }
    }

    void EditorUI::DrawOverlay()
    {
        if (!m_showMetrics)
        {
            return;
        }

        Application& app = GetApplication();
        const FrameTimer& timer = app.GetFrameTimer();
        const RenderStats& stats = app.GetRenderer().GetStats();

        constexpr ImGuiWindowFlags kFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove
                                          | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
                                          | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize
                                          | ImGuiWindowFlags_NoInputs;

        // Pinned to the top right, so it never sits under the Dev Menu at its default position.
        const float pad = ImGui::GetFontSize();
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - pad, pad), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
        ImGui::SetNextWindowBgAlpha(0.35f);

        if (ImGui::Begin("Metrics", nullptr, kFlags))
        {
            ImGui::Text("FPS         %u", timer.GetFPS());
            ImGui::Text("Frame       %6.2f ms", timer.GetDeltaSeconds() * 1000.0);

            // CPU spans the whole frame including the drawable block, so CPU minus Wait is the
            // work actually done.
            ImGui::Text("CPU         %6.2f ms", timer.GetCpuSeconds() * 1000.0);
            ImGui::Text("GPU         %6.2f ms", stats.gpuMs);
            ImGui::Text("Wait        %6.2f ms", stats.waitMs);

            ImGui::Separator();
            ImGui::Text("Draw Calls  %u", stats.drawCalls);
            ImGui::Text("Triangles   %u", stats.triangles);
            ImGui::Text("Memory      %6.1f MiB", static_cast<float64>(stats.memoryBytes) / (1024.0 * 1024.0));
        }

        ImGui::End();
    }
}
