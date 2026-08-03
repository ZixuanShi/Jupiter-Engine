// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Camera;

import jpt.Math;
import jpt.Matrix44;
import jpt.TypeDefs;
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
        static constexpr ProjectionMode kProjectionMode = ProjectionMode::Perspective;

        Vec3 m_position = Vec3::Zero();
        Vec3 m_target   = Vec3::Forward();

        // Both modes are driven by a vertical extent and derive the horizontal from aspect, so
        // switching between them keeps the framing and changes only the convergence.
        float32 m_fovY        = ToRadians(60.0f);
        float32 m_orthoHeight = 3.7f;           // World units top to bottom. Matches fovY at ~3.2 units out.
        float32 m_zNear       = 0.1f;
        float32 m_zFar        = 100.0f;

    public:
        void SetPosition(const Vec3& position) noexcept;
        void SetTarget(const Vec3& target) noexcept;

        [[nodiscard]] Mat44 GetViewProjection(float32 aspect) const noexcept;
    };
}
