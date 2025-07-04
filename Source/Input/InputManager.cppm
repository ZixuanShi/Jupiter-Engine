// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"

export module jpt.InputManager;

import jpt.TypeDefs;

import jpt.RawInput;

import jpt.InputManager_Create;

import jpt.Framework_Enums;

export namespace jpt
{
    class InputManager
    {
        JPT_DECLARE_SINGLETON(InputManager);

    private:
        Input::RawInput* m_pRawInput = nullptr;

    public:
        bool PreInit(FrameworkAPI api);
        bool Init();
        void Update(TimePrecision deltaSeconds);
        void Terminate();

    public:
        Input::RawInput* GetRawInput() const { return m_pRawInput; }
    };

    bool InputManager::PreInit(FrameworkAPI api)
    {
        m_pRawInput  = Input::RawInput_Create(api);

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
        Allocator<Input::RawInput>::Delete(m_pRawInput);
        m_pRawInput = nullptr;
    }
}