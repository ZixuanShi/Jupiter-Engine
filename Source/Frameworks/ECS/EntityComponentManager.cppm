// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.EntityComponentManager;

import jpt.EntityComponent;

import jpt.TypeDefs;
import jpt.TimeTypeDefs;

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