// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#include "Enum.h"

#include "Debugging/Logger.h"

using namespace jpt;

void BuildNames(jpt::HashMap<uint8, jpt::String>& namesMap, uint8& count, uint8& start, const char* pSource)
{

}


/*

private:\
	void BuildNames()\
	{\
		jpt::String source(#__VA_ARGS__);\
		source.Replace(" ", "");\
	\
		jpt::DynamicArray<jpt::String> splittedNames = source.Split(',');\
	\
		for (size_t i = 0; i < splittedNames.Size(); ++i)\
		{\
			m_names[i] = Move(splittedNames[i]);\
		}\
	}\

*/