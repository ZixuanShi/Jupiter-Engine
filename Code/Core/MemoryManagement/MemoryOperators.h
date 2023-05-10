#pragma once

#define JPT_DELETE(pPointer)\
			delete pPointer;\
			pPointer = nullptr;
#define JPT_DELETE_ARRAY(pPointer)\
			delete[] pPointer;\
			pPointer = nullptr;

#define JPT_SAFE_DELETE(pPointer)\
		if (pPointer)\
		{\
			JPT_DELETE(pPointer);\
		}
#define JPT_SAFE_DELETE_ARRAY(pPointer)\
		if (pPointer)\
		{\
			JPT_DELETE_ARRAY(pPointer);\
		}