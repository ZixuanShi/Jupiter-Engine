// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Entity.Component.Manager;

import jpt.Entity.Component;

import jpt.TypeDefs;
import jpt.Time.TypeDefs;

export namespace jpt
{
    /** Manages all entity components in current scene */
    class EntityComponentManager
    {
    private:


    public:
        void Update(TimePrecision deltaSeconds);
    };

    void EntityComponentManager::Update(TimePrecision)
    {

    }
}