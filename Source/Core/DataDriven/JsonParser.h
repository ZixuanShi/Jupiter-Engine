// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	class JPT_API JsonParser
	{
	public:
		static JsonParser& GetInstance();

	private:
		JsonParser() = default;
		JsonParser(const JsonParser&) = delete;
		JsonParser& operator=(const JsonParser&) = delete;
	};
}