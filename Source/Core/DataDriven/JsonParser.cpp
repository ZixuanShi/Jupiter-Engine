// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "JupiterPCH.h"
#include "JsonParser.h"

namespace jpt
{
	JsonParser& jpt::JsonParser::GetInstance()
	{
		static JsonParser s_instance;
		return s_instance;
	}
}
