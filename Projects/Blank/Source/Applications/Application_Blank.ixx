// Copyright Jupiter Technologies, Inc. All Rights Reserved.


export module Application_Blank;

import jpt.Time.TypeDefs;

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
	virtual void Update(jpt::TimePrecision deltaSeconds) override;
	virtual void Shutdown() override;
};