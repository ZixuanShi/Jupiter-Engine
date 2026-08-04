// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Camera;

import jpt.Math;
import jpt.Matrix44;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

export namespace jpt
{
    enum class ProjectionMode : uint8
    {
        Perspective,    // Converges with distance. What a game view wants.
        Orthographic,   // Parallel lines stay parallel. What an editor's front/side/top wants.
    };

    /** Right-handed, looking down -Z. Aspect is not a member: it belongs to the surface being
        drawn to rather than to the camera, so it arrives per call.

        Aimed by a target point rather than a rotation, which is what an orbit camera pivots
        around and what a gizmo drags. */
    class Camera
    {
    private:
        ProjectionMode m_projectionMode = ProjectionMode::Perspective;

        Vec3 m_position = Vec3(1.6f, 2.0f, 2.4f);
        Vec3 m_target   = Vec3::Zero();

        // Both modes are driven by a vertical extent and derive the horizontal from aspect, so
        // switching between them keeps the framing and changes only the convergence.
        float32 m_fovY        = ToRadians(60.0f);
        float32 m_orthoHeight = 3.7f;           // World units top to bottom. Matches fovY at ~3.2 units out.
        float32 m_zNear       = 0.1f;
        float32 m_zFar        = 100.0f;

    public:
        bool Init();

    public:
        /** Scales the distance to the target, and orthoHeight with it so a mode switch keeps
            the framing. Clamped, so a fast pinch cannot bury the camera in the target. */
        void Zoom(float32 factor) noexcept;

        /** World offset for a screen drag at the target's depth, so a dragged object tracks the
            pointer. deltaPixels and viewportHeight must be the same unit. */
        [[nodiscard]] Vec3 ScreenDeltaToWorld(const Vec2& deltaPixels, float32 viewportHeight) const noexcept;

    public:
        void SetPosition(const Vec3& position) noexcept;
        void SetTarget(const Vec3& target) noexcept;

        void SetProjectionMode(ProjectionMode mode) noexcept;
        void SetFovY(float32 radians) noexcept;
        void SetOrthoHeight(float32 worldUnits) noexcept;
        void SetNearFar(float32 zNear, float32 zFar) noexcept;

        [[nodiscard]] ProjectionMode GetProjectionMode() const noexcept { return m_projectionMode; }
        [[nodiscard]] float32 GetFovY()        const noexcept { return m_fovY; }
        [[nodiscard]] float32 GetOrthoHeight() const noexcept { return m_orthoHeight; }
        [[nodiscard]] float32 GetNear()        const noexcept { return m_zNear; }
        [[nodiscard]] float32 GetFar()         const noexcept { return m_zFar; }

        [[nodiscard]] Mat44 GetViewProjection(float32 aspect) const noexcept;
    };
}
