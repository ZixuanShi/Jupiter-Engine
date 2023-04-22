#pragma once

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