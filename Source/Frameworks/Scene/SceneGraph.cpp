// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module jpt.SceneGraph;

namespace jpt
{
    void SceneGraph::Update(TimePrecision deltaSeconds)
    {
        for (Index id : m_activeEntities)
        {
            m_entityPool[id]->Update(deltaSeconds);
        }

        m_componentManager.Update(deltaSeconds);
    }
}