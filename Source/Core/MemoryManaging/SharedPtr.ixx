// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.SharedPtr;

import jpt.TypeDefs;

export namespace jpt
{
	template<typename DataT>
	class SharedPtr
	{
	private:
		DataT* m_pPtr = nullptr;
		int32* m_refCount = nullptr;
	};
}