#pragma once

#define JPT_RAW_NEW	new

#define JPT_NEW(param) new param

// Deletes a pointer no matter what it's pointing, then sets it to nullptr
#define JPT_DELETE(pPointer)\
	delete pPointer;\
	pPointer = nullptr;

// Deletes a pointer if it's valid
#define JPT_SAFE_DELETE(pPointer)\
	if (pPointer)\
	{\
		JPT_DELETE(pPointer)\
	}