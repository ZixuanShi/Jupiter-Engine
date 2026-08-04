// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

// Textual before the imports. AppClient.h here would close a cycle in EditorUI.cppm, but an
// implementation unit may reach the application.
#include "imgui.h"
#include "Applications/AppClient.h"
#include "Graphics/Renderer.h"

module jpt.EditorUI;

import jpt.Camera;
import jpt.LinearColor;
import jpt.Math;
import jpt.Scene;
import jpt.TypeDefs;
import jpt.Vector3;
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

            const Vec3& position = camera.GetPosition();
            const Vec3 forward = camera.Forward();
            ImGui::Text("At   %6.2f %6.2f %6.2f", position.x, position.y, position.z);
            ImGui::Text("Look %6.2f %6.2f %6.2f", forward.x, forward.y, forward.z);

            // Relative, not absolute: each field sits at zero, a drag applies that frame's delta
            // along the camera's own axes, and it returns to zero. Editing a world-space position
            // meant a drag on X slid the camera along world X no matter where it was pointing.
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

        void DrawBackgroundSection(Renderer& renderer)
        {
            if (!ImGui::CollapsingHeader("Background", ImGuiTreeNodeFlags_DefaultOpen))
            {
                return;
            }

            LinearColor color = renderer.GetClearColor();
            if (ImGui::ColorEdit3("Color", &color.r, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR))
            {
                renderer.SetClearColor(color);
            }
        }
    }

    void DrawEditorUI()
    {
        Application& app = GetApplication();

        const float fontSize = ImGui::GetFontSize();
        ImGui::SetNextWindowPos(ImVec2(fontSize, fontSize), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(fontSize * 26.0f, 0.0f), ImGuiCond_FirstUseEver);   // 0 height: fit to content.

        if (ImGui::Begin("Dev Menu"))
        {
            DrawCameraSection(app.GetScene().GetCamera());
            DrawBackgroundSection(app.GetRenderer());
        }

        ImGui::End();
    }
}
