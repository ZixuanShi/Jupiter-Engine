// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module Application_Blank;

import jpt.TypeDefs;

import jpt.Application;
import Entity.Foo;
import Entity.Bar;

export class Application_Blank final : public jpt::Application
{
    using Super = jpt::Application;

private:
    Entity_Foo* m_pFoo;

public:
    virtual bool PreInit() override;
    virtual void Update(TimePrecision deltaSeconds) override;
    virtual void Terminate() override;
};