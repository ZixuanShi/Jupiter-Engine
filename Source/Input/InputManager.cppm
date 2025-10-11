// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Memory/Memory.h"
#include "Core/Minimal/Utilities.h"

export module jpt.InputManager;

import jpt.TypeDefs;
import jpt.RawInput;
import jpt.InputManager_Create;
import jpt.Framework_Enums;

namespace jpt
{
    export class InputManager
    {
        JPT_DECLARE_SINGLETON(InputManager);

    private:
        Input::RawInput* m_pRawInput = nullptr;

    public:
        bool PreInit();
        bool Init();
        void Update(TimePrecision deltaSeconds);
        void Terminate();

    public:
        Input::RawInput* GetRawInput() const { return m_pRawInput; }
    };
}