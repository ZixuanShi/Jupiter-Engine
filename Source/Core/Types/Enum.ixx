// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Debugging/Assert.h"
#include "Debugging/Logger.h"

#include <stdio.h>
#include <stdarg.h>

export module jpt.Enum;

import jpt.StringUtils;

export namespace jpt
{
	/** An Enum class supports the following operations: 
		-  
		
		@note	This enum costs performance. When you expect your enum just do trivial stuff, use the ordinary one instead */
	class Enum
	{
	public:
		Enum(const char* arg, ...);
	};

	Enum::Enum(const char* arg, ...)
	{
		va_list arguments;

		for (va_start(arguments, arg); 
			arg != nullptr; 
			arg = va_arg(arguments, const char*))
		{
			if (IsValidDataCStr(arg, false) && GetCStrLength(arg) > 0)
			{
				JPT_LOG(arg);

			}
		}

		va_end(arguments);
	}
}