// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Light;

import jpt.LinearColor;
import jpt.TypeDefs;
import jpt.Vector3;

export namespace jpt
{
    /** Light bulb, emitting light equally in all directions from a single, infinitely small point in space */
    struct PointLight
    {
        Vec3 position     = Vec3::Zero();
        LinearColor color = LinearColor::White();
        float32 intensity = 1.0f;
        bool enabled      = true;
    };

    /** Irradiance from a uniform sky dome above and a ground plane below, blended by the normal */
    struct AmbientLight
    {
        LinearColor sky    = LinearColor(0.16f, 0.20f, 0.28f);
        LinearColor ground = LinearColor(0.12f, 0.09f, 0.06f);
    };
}
