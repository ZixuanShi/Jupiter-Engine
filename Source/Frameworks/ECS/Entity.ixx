// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Entity;

import jpt.String;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.TimeTypeDefs;

export namespace jpt
{
    /** Represents a game object. Can be inherited and change Life-Cycle behaviors */
    class Entity
    {
    protected:
        String m_name = "Entity";        /**< The name of the entity */

    public:
        virtual ~Entity() = default;

        virtual bool PreInit() { return true; }
        virtual bool Init() { return true; }
        virtual void Update(TimePrecision) {}
        virtual void Shutdown() {}
    };
}