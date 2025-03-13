// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Particle;

import jpt.TypeDefs;
import jpt.Concepts;

import jpt.Vector2;
import jpt.LinearColor;

export namespace jpt
{
    /** A 2D particle on screen */
    template<Floating T>
    struct TParticle2
    {
        Vector2<T> position;
        Vector2<T> velocity;
        LinearColor color;
    };
}

export using Particle2 = jpt::TParticle2<Precision>;