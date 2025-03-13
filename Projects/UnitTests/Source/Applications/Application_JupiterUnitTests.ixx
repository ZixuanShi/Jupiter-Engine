// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module Application_JupiterUnitTests;

import jpt.Application;

export class Application_JupiterUnitTests final : public jpt::Application
{
private:
    using Super = jpt::Application;

public:
    virtual bool PreInit() override;
    virtual bool Init() override;
};

