// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module Application_Benchmarks;

import jpt.Application;

export class Application_Benchmarks final : public jpt::Application
{
private:
	using Super = jpt::Application;

public:
	virtual bool PreInit() override;
	virtual bool Init() override;
};