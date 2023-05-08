#pragma once

#if !JPT_ENABLE_MEMORY_TRACKING
// Not tracking memory versions. Fast and efficient
#define JPT_NEW(type) new type()
#define JPT_NEW_ARRAY(type, size) new type[size]
#define JPT_NEW_WITH_VALUE(type, initialValue) new type(initialValue)

// Deletes a pointer no matter what it's pointing, then sets it to nullptr
#define JPT_DELETE(pPointer)\
			delete pPointer;\
			pPointer = nullptr;
#define JPT_DELETE_ARRAY(pPointer)\
			delete[] pPointer;\
			pPointer = nullptr;

#elif JPT_ENABLE_MEMORY_TRACKING
namespace jpt
{
	// JPT_NEW Macro implementation if JPT_ENABLE_MEMORY_TRACKING is on. This will track the memory we allocated
	// Should not be directly called, use JPT_NEW() instead, as this will not exist if JPT_ENABLE_MEMORY_TRACKING is off
	// - template Type: The type of the pointer to return
	// - optinal initialValue: Optional parameter to initialize the pointer's underhood value
	// Returns: The pointer of the type we requested to allocate
	template<typename Type>
	inline Type* InternalCallTrackedNew(const char* pFileName, int32 lineNum)
	{ 
		JPT_UNUSED(pFileName);
		JPT_UNUSED(lineNum);

		Type* pPointer = new Type();
		//MemoryTracker::GetInstance()->AddEntry(pFileName, lineNum, pPointer);
		return pPointer;
	}
	template<typename Type>
	inline Type* InternalCallTrackedNew(Type&& initialValue, const char* pFileName, int32 lineNum)
	{
		JPT_UNUSED(pFileName);
		JPT_UNUSED(lineNum);

		Type* pPointer = new Type(std::move(initialValue));
		//MemoryTracker::GetInstance()->AddEntry(pFileName, lineNum, pPointer);
		return pPointer;
	}
	template<typename Type>
	inline Type* InternalCallTrackedNew(const Type& initialValue, const char* pFileName, int32 lineNum)
	{
		JPT_UNUSED(pFileName);
		JPT_UNUSED(lineNum);

		Type* pPointer = new Type(initialValue);
		//MemoryTracker::GetInstance()->AddEntry(pFileName, lineNum, pPointer);
		return pPointer;
	}
	template<typename Type>
	inline Type* InternalCallTrackedNewArray(size_t size, const char* pFileName, int32 lineNum)
	{
		JPT_UNUSED(pFileName);
		JPT_UNUSED(lineNum);

		Type* pPointer = new Type[size];
		//MemoryTracker::GetInstance()->AddEntry(pFileName, lineNum, pPointer);
		return pPointer;
	}

	JPT_API void InternalCallTrackedDelete(void* pPointer);
	JPT_API void InternalCallTrackedDeleteArray(void* pPointer);

	// Only type, no initialize value
	// Example: int32* pInt = JPT_NEW(int32); equivalent to int32* pInt = new int32();
	#define JPT_NEW(type) jpt::InternalCallTrackedNew<type>(__FILE__, __LINE__)

	// Equivalent to 
	#define JPT_NEW_ARRAY(type, size) jpt::InternalCallTrackedNewArray<type>(size, __FILE__, __LINE__);

	// New with an initializing value
	// Example: int32* pInt = JPT_NEW(int32, 42); equivalent to int32* pInt = new int32(42);
	#define JPT_NEW_WITH_VALUE(type, initialValue) jpt::InternalCallTrackedNew<type>(initialValue, __FILE__, __LINE__)

	// Deletes a pointer no matter what it's pointing, then sets it to nullptr
	#define JPT_DELETE(pPointer) jpt::InternalCallTrackedDelete(pPointer)

	// Deletes a pointer no matter what it's pointing, then sets it to nullptr
	#define JPT_DELETE_ARRAY(pPointer) jpt::InternalCallTrackedDeleteArray(pPointer)
}

#endif // JPT_ENABLE_MEMORY_TRACKING

// Deletes a pointer if it's valid
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