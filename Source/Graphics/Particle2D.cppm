// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <concepts>

export module jpt.Particle2D;

import jpt.TypeDefs;
import jpt.Concepts;

import jpt.Vector2;
import jpt.LinearColor;

export namespace jpt
{
    /** A 2D particle on screen */
    template<std::floating_point T>
    struct TParticle2D
    {
        Vector2<T> position;
        Vector2<T> velocity;
        LinearColor color;
    };
}

export using Particle2D = jpt::TParticle2D<Precision>;