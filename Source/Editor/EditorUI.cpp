// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// Textual first, then the imports: the rule that already governs Metal4Renderer.cpp. imgui.h is
// plain C++ and includes no STL, so it costs little here -- measured before wrapping it.
#include "imgui.h"

module jpt.EditorUI;

import jpt.Camera;
import jpt.Math;
import jpt.TypeDefs;
import std;

namespace jpt
{
    namespace
    {
        void DrawCameraSection(Camera& camera)
        {
            if (!ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
            {
                return;
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
    }

    void DrawEditorUI(Camera& camera)
    {
        const float fontSize = ImGui::GetFontSize();
        ImGui::SetNextWindowPos(ImVec2(fontSize, fontSize), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(fontSize * 26.0f, 0.0f), ImGuiCond_FirstUseEver);   // 0 height: fit to content.

        if (ImGui::Begin("Dev Menu"))
        {
            DrawCameraSection(camera);
        }

        ImGui::End();
    }
}
