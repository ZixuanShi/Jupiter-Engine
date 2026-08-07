// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// Textual before the imports, and ShaderTypes.h before Renderer.h for the same reason: it pulls
// <simd/simd.h>, and a libc++ header included after a module has declared it is a redefinition.
// AppClient.h here would close a cycle in EditorUI.cppm, but an implementation unit may reach
// the application.
#include "imgui.h"
#include "Graphics/Shader/ShaderTypes.h"
#include "Applications/AppClient.h"
#include "Applications/Window/Window.h"
#include "Graphics/Renderer.h"

module jpt.EditorUI;

import jpt.Camera;
import jpt.FrameTimer;
import jpt.Light;
import jpt.LinearColor;
import jpt.Material;
import jpt.Math;
import jpt.Matrix44;
import jpt.Scene;
import jpt.TypeDefs;
import jpt.Vector3;
import jpt.Vector4;
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

            const std::array<PointLight, kMaxPointLights>& lights = GetApplication().GetScene().GetPointLights();
            for (usize i = 0; i < lights.size(); ++i)
            {
                DrawPointLight(i);
            }
            DrawVfx();
            DrawCamera();
        }

        ImGui::End();

        // Outside the panel: it draws into the background list, not into a window.
        DrawPointLightGizmos();
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

        // ShaderTypes.h owns the ordering, because the shader switches on it. This only names it.
        static constexpr const char* kViewModes[] =
        {
            "Final", "Base Color", "Normal", "Roughness", "Metallic",
        };
        static_assert(std::size(kViewModes) == static_cast<usize>(ViewMode::Count), "View mode names are out of sync");

        // Isolating a term is what tells "the slider did nothing" apart from "the term is not
        // reaching the shader at all" -- indistinguishable in the Final view.
        int32 viewMode = static_cast<int32>(material.viewMode);
        if (ImGui::Combo("View", &viewMode, kViewModes, IM_ARRAYSIZE(kViewModes)))
        {
            material.viewMode = static_cast<uint32>(viewMode);
        }

        ImGui::ColorEdit3("Base Color", &material.baseColor.r, ImGuiColorEditFlags_Float);
        ImGui::SliderFloat("Roughness", &material.roughness, 0.0f, 1.0f);
        ImGui::SliderFloat("Metallic", &material.metallic, 0.0f, 1.0f);
        ImGui::SliderFloat("Ambient Occlusion", &material.occlusion, 0.0f, 1.0f);
    }

    void EditorUI::DrawPointLight(usize index)
    {
        if (!ImGui::CollapsingHeader(std::format("PointLight {}", index + 1).c_str()))
        {
            return;
        }

        PointLight& light = GetApplication().GetScene().GetPointLights()[index];

        // ImGui keys a widget by its label, so three sections of identically named sliders
        // would be one slider driving all three lights.
        ImGui::PushID(static_cast<int32>(index));

        ImGui::Checkbox("Enabled", &light.enabled);
        ImGui::DragFloat("Intensity", &light.intensity, 1.0f, 0.0f, 1000.0f, "%.0f");
        ImGui::ColorEdit3("Color", &light.color.r, ImGuiColorEditFlags_Float);
        ImGui::DragFloat3("Position", &light.position.x, 0.02f);

        ImGui::PopID();
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

    /** A marker per light: where it is, whether it is on, and which one it is */
    void EditorUI::DrawPointLightGizmos()
    {
        Application& app = GetApplication();
        std::span<const PointLight> lights = app.GetScene().GetPointLights();

        // Behind every ImGui window, so the Dev Menu covers the markers rather than the reverse.
        ImDrawList* pDrawList = ImGui::GetBackgroundDrawList();
        const Mat44 viewProjection = app.GetScene().GetCamera().GetViewProjection(app.GetWindow().GetAspectRatio());

        // ImGui works in points and the engine's window is pixels. Using the window size here
        // puts every marker at twice its position on a Retina display.
        const ImVec2 display = ImGui::GetIO().DisplaySize;

        constexpr float32 kRadius = 6.0f;

        for (usize i = 0; i < lights.size(); ++i)
        {
            const PointLight& light = lights[i];
            const Vec4 clip = viewProjection * Vec4(light.position, 1.0f);

            // Behind the eye. The perspective divide would fold it back onto the screen,
            // mirrored, which reads as a light sitting exactly where it is not.
            if (clip.w <= 0.0f)
            {
                continue;
            }

            // Metal's NDC is +Y up and a screen is +Y down, so y is the only axis that flips.
            const float32 invW = 1.0f / clip.w;
            const ImVec2 screen((clip.x * invW * 0.5f + 0.5f) * display.x,
                                (0.5f - clip.y * invW * 0.5f) * display.y);

            const ImU32 color = ImGui::GetColorU32(ImVec4(light.color.r, light.color.g, light.color.b, 1.0f));

            // Solid when on, hollow when off. A six-pixel dot is too small for a dimmed one to
            // read as anything but a dimmer light.
            if (light.enabled)
            {
                pDrawList->AddCircleFilled(screen, kRadius, color);
            }

            pDrawList->AddCircle(screen, kRadius, color, 0, 2.0f);
            pDrawList->AddText(ImVec2(screen.x + kRadius + 4.0f, screen.y - ImGui::GetFontSize() * 0.5f),
                               color, std::format("{}", i + 1).c_str());
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
