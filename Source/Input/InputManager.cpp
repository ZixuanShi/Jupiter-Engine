// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Memory/Memory.h"

module jpt.InputManager;

namespace jpt
{
    bool InputManager::PreInit()
    {
        m_pRawInput = Input::RawInput_Create();

        bool success = true;
        success &= m_pRawInput->PreInit();
        return success;
    }

    bool InputManager::Init()
    {
        bool success = true;

        success &= m_pRawInput->Init();
        return success;
    }

    void InputManager::Update(TimePrecision deltaSeconds)
    {
        m_pRawInput->Update(deltaSeconds);
    }

    void InputManager::Terminate()
    {
        JPT_DELETE(m_pRawInput);
        m_pRawInput = nullptr;
    }
}