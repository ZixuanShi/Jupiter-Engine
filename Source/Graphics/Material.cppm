// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Material;

import jpt.LinearColor;
import jpt.TypeDefs;

export namespace jpt
{
    /** How a surface answers light */
    struct Material
    {
        LinearColor baseColor = LinearColor::White();   // Multiplies the base colour map

        /** Dissolve VFX. dissolvePct is the progress, 0 intact to 1 gone; Scene::Update() moves it
            toward whichever end dissolving points at, dissolveSpeed units per second. */
        LinearColor dissolveColor = LinearColor(1.0f, 0.35f, 0.05f);
        float32 dissolvePct   = 0.0f;
        float32 dissolveSpeed = 1.0f;
        float32 dissolveEdge  = 0.03f;   // Burning rim width, in noise units
        bool    dissolving    = false;
    };
}
