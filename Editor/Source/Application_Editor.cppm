// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module Application_Editor;

import jpt.Application;
import jpt.Logger;

export class Application_Editor final : public jpt::Application
{
public:
    virtual bool PreInit() override
    {
        jpt::Debug("This is {} {}", JUPITER_PROJECT_NAME, JUPITER_PRESET);
        return true;
    }
};
