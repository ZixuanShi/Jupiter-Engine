// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Camera;

import jpt.Input;
import jpt.Math;
import jpt.Matrix44;
import jpt.Quaternion;
import jpt.TypeDefs;
import jpt.Vector2;
import jpt.Vector3;

export namespace jpt
{
    /** WASD, QE and the arrows in the camera's own axes: x right, y up, z backward, so W is -z.
        Local throughout -- E rises along the camera's up, which is only world up while it is
        looking level. Free and taking Input rather than reading it inside Camera, so the key
        mapping is testable without an Application. */
    [[nodiscard]] Vec3 GetMoveAxis(const Input& input) noexcept;

    enum class ProjectionMode : uint8
    {
        Perspective,    // Converges with distance. What a game view wants.
        Orthographic,   // Parallel lines stay parallel. What an editor's front/side/top wants.
    };

    /** Right-handed, looking down -Z. Aspect is not a member: it belongs to the surface being
        drawn to rather than to the camera, so it arrives per call.

        Position and rotation, which is a Transform minus the scale a camera has no meaning for.
        The distance alongside them is not bookkeeping: it is the radius Zoom scales and clamps,
        and the depth a screen drag is projected through.

        Defaults are neutral on purpose. Where a viewer starts is a fact about a particular scene
        -- it is only sensible relative to what is in the world -- so the scene places it.

        The rotation is always yaw-then-pitch and never carries roll -- see RotateLocal. */
    class Camera
    {
    private:
        ProjectionMode m_projectionMode = ProjectionMode::Perspective;
        Vec3 m_position       = Vec3::Zero();
        Quat m_rotation       = Quat::Identity();   // Looking down -Z.
        float32 m_distance    = 1.0f;
        float32 m_fovY        = ToRadians(60.0f);
        float32 m_orthoHeight = 3.7f;           // World units top to bottom. Matches fovY at ~3.2 units out.
        float32 m_zNear       = 0.1f;
        float32 m_zFar        = 100.0f;

    public:
        bool Init();
        void Update();

    public:
        void MoveLocal(const Vec3& offset) noexcept;
        void RotateLocal(float32 pitchRadians, float32 yawRadians) noexcept;
        void Zoom(float32 factor) noexcept;
        void LookAt(const Vec3& point) noexcept;

        /** The camera's world-space axes. Screen right and screen up, and where it looks. */
        [[nodiscard]] Vec3 Right()    const noexcept { return m_rotation.Right(); }
        [[nodiscard]] Vec3 Up()       const noexcept { return m_rotation.Up(); }
        [[nodiscard]] Vec3 Forward()  const noexcept { return m_rotation.Forward(); }
        [[nodiscard]] Vec3 Backward() const noexcept { return m_rotation.Backward(); }

        /** World offset for a screen drag at the orbit distance, so a dragged object tracks the
            pointer. deltaPixels and viewportHeight must be the same unit. */
        [[nodiscard]] Vec3 ScreenDeltaToWorld(const Vec2& deltaPixels, float32 viewportHeight) const noexcept;

    public:
        void SetPosition(const Vec3& position) noexcept;
        void SetDirection(const Vec3& direction) noexcept;   // Normalized on the way in.
        void SetDistance(float32 distance) noexcept;

        void SetProjectionMode(ProjectionMode mode) noexcept;
        void SetFovY(float32 radians) noexcept;
        void SetOrthoHeight(float32 worldUnits) noexcept;
        void SetNearFar(float32 zNear, float32 zFar) noexcept;

        [[nodiscard]] const Vec3& GetPosition()  const noexcept { return m_position; }
        [[nodiscard]] const Quat& GetRotation()  const noexcept { return m_rotation; }
        [[nodiscard]] Vec3 GetDirection()        const noexcept { return Forward(); }
        [[nodiscard]] float32 GetDistance()      const noexcept { return m_distance; }

        [[nodiscard]] ProjectionMode GetProjectionMode() const noexcept { return m_projectionMode; }
        [[nodiscard]] float32 GetFovY()        const noexcept { return m_fovY; }
        [[nodiscard]] float32 GetOrthoHeight() const noexcept { return m_orthoHeight; }
        [[nodiscard]] float32 GetNear()        const noexcept { return m_zNear; }
        [[nodiscard]] float32 GetFar()         const noexcept { return m_zFar; }

        [[nodiscard]] Mat44 GetViewProjection(float32 aspect) const noexcept;

    private:
        void Look(const Vec2& deltaPixels);
    };
}
