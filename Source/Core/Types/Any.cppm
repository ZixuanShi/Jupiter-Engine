// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"
#include "Core/Validation/Assert.h"

#include <cstring>

export module jpt.Any;

import jpt.Allocator;
import jpt.Byte;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.TypeRegistry;
import jpt.Utilities;

static constexpr size_t kLocSmallDataSize = 8;

export namespace jpt
{
    /** Can hold any type */
    class Any
    {
        using Constructor = void(*)(Byte*, const Byte*);
        using Destructor  = void(*)(Byte*);

    private:
        Byte m_smallBuffer[kLocSmallDataSize]; /**< Small Object Optimization. Will hold data in this local stack buffer if it's small */
        Byte* m_pBuffer = nullptr;             /**< Buffer pointer to the memory of hold object */

        Constructor m_constructor = nullptr;   /**< Constructor function pointer. Needed for copy constructing */
        Destructor  m_destructor  = nullptr;   /**< Destructor function pointer. Needed for destructing */

        Id m_currentTypeId       = 0;       /**< Id of the current type. Used for comparing */
        size_t m_currentTypeSize = 0;       /**< Size in bytes of the current type */

    public:
        constexpr Any() = default;
        constexpr ~Any();
        Any(const Any& other);
        Any(Any&& other) noexcept;
        Any& operator=(const Any& other);
        Any& operator=(Any&& other) noexcept;

        template<typename T> 
        requires (!AreSameType<T, Any>)
        constexpr Any(const T& value);

        template<typename T> 
        requires (!AreSameType<T, Any>) && AreSameType<T, TRemoveReference<T>>
        constexpr Any(T&& value);

        template<typename T> 
        requires (!AreSameType<T, Any>)
        constexpr Any& operator=(const T& value);

        template<typename T> 
        requires (!AreSameType<T, Any>) && AreSameType<T, TRemoveReference<T>>
        constexpr Any& operator=(T&& value);

        template<typename T> 
        constexpr T& As();

        template<typename T> 
        constexpr const T& As() const;

        template<typename T> 
        constexpr bool Is() const;

        constexpr bool IsEmpty() const;
        constexpr void Clear();

    private:
        void CopyAny(const Any& other);
        void MoveAny(Any&& other);

        template<typename T> 
        requires (!AreSameType<T, Any>)
        constexpr void ConstructType(const T& value);

        template<typename T> 
        requires (!AreSameType<T, Any>) && AreSameType<T, TRemoveReference<T>>
        constexpr void ConstructType(T&& value);

        /** Adapt to another Type, that type is guaranteed not the same as current type */
        template<typename T> 
        requires (!AreSameType<T, Any>)
        constexpr void Adapt();

        constexpr void DestructObject();
        constexpr void DeallocateBuffer();
    };

    constexpr Any::~Any()
    {
        Clear();
    }

    Any::Any(const Any& other)
    {
        CopyAny(other);
    }

    Any::Any(Any&& other) noexcept
    {
        MoveAny(Move(other));
    }

    Any& Any::operator=(const Any& other)
    {
        if (this != &other)
        {
            Clear();
            CopyAny(other);
        }

        return *this;
    }

    Any& Any::operator=(Any&& other) noexcept
    {
        if (this != &other)
        {
            Clear();
            MoveAny(Move(other));
        }

        return *this;
    }

    constexpr void Any::DestructObject()
    {
        if (m_destructor)
        {
            m_destructor(m_pBuffer);
        }
    }

    constexpr void Any::DeallocateBuffer()
    {
        if (m_pBuffer &&
            m_pBuffer != m_smallBuffer)
        {
            Allocator<Byte>::DeallocateArray(m_pBuffer);
        }
    }

    template<typename T>
    requires (!AreSameType<T, Any>)
    constexpr Any::Any(const T& value)
    {
        ConstructType(value);
    }

    template<typename T>
    requires (!AreSameType<T, Any>) && AreSameType<T, TRemoveReference<T>>
    constexpr Any::Any(T&& value)
    {
        ConstructType(Move(value));
    }

    template<typename T>
    requires (!AreSameType<T, Any>)
    constexpr Any& Any::operator=(const T& value) 
    {
        if (Is<T>())
        {
            As<T>() = value;
            return *this;
        }

        DestructObject();
        ConstructType(value);
        return *this;
    }

    template<typename T>
    requires (!AreSameType<T, Any>) && AreSameType<T, TRemoveReference<T>>
    constexpr Any& Any::operator=(T&& value)
    {
        if (Is<T>())
        {
            As<T>() = Move(value);
            return *this;
        }

        DestructObject();
        ConstructType(Move(value));
        return *this;
    }

    template<typename T>
    constexpr T& Any::As()
    {
        JPT_ASSERT(Is<T>(), "Any should be assigned to the given T before calling As()");
        return reinterpret_cast<T&>(*m_pBuffer);
    }

    template<typename T>
    constexpr const T& Any::As() const
    {
        JPT_ASSERT(Is<T>(), "Any should be assigned to the given T before calling As()");
        return reinterpret_cast<const T&>(*m_pBuffer);
    }

    template<typename T>
    constexpr bool Any::Is() const
    {
        return m_currentTypeSize == sizeof(T) && m_currentTypeId == TypeRegistry::GetId<T>();
    }

    constexpr bool Any::IsEmpty() const
    {
        return m_currentTypeSize == 0 || 
               m_currentTypeId == 0 || 
               m_pBuffer == nullptr;
    }

    constexpr void Any::Clear()
    {
        DestructObject();
        DeallocateBuffer();
        m_pBuffer     = nullptr;
        m_constructor = nullptr;
        m_destructor  = nullptr;
        m_currentTypeId = 0;
        m_currentTypeSize = 0;
    }

    void Any::CopyAny(const Any& other)
    {
        m_currentTypeId = other.m_currentTypeId;
        m_currentTypeSize = other.m_currentTypeSize;

        if (m_currentTypeSize <= kLocSmallDataSize)
        {
            m_pBuffer = m_smallBuffer;
            MemCpy(m_pBuffer, other.m_smallBuffer, kLocSmallDataSize);
        }
        else
        {
            m_pBuffer = Allocator<Byte>::AllocateArray(m_currentTypeSize);
            MemCpy(m_pBuffer, other.m_pBuffer, m_currentTypeSize);
        }

        m_constructor = other.m_constructor;
        m_destructor  = other.m_destructor;

        if (m_constructor)
        {
            m_constructor(m_pBuffer, other.m_pBuffer);
        }
    }

    void Any::MoveAny(Any&& other)
    {
        m_currentTypeId = other.m_currentTypeId;
        m_currentTypeSize = other.m_currentTypeSize;

        MemCpy(m_smallBuffer, other.m_smallBuffer, kLocSmallDataSize);
        if (m_currentTypeSize <= kLocSmallDataSize)
        {
            m_pBuffer = m_smallBuffer;
        }
        else
        {
            m_pBuffer = other.m_pBuffer;
        }

        m_constructor = other.m_constructor;
        m_destructor  = other.m_destructor;

        other.m_pBuffer     = nullptr;
        other.m_constructor = nullptr;
        other.m_destructor  = nullptr;
        other.m_currentTypeId   = 0;
        other.m_currentTypeSize = 0;
    }

    template<typename T>
    requires (!AreSameType<T, Any>)
    constexpr void Any::ConstructType(const T& value)
    {
        Adapt<T>();
        Allocator<T>::Construct(reinterpret_cast<T*>(m_pBuffer), value);
    }

    template<typename T>
    requires (!AreSameType<T, Any>) && AreSameType<T, TRemoveReference<T>>
    constexpr void Any::ConstructType(T&& value)
    {
        Adapt<T>();
        Allocator<T>::Construct(reinterpret_cast<T*>(m_pBuffer), Move(value));
    }

    template<typename T>
    requires (!AreSameType<T, Any>)
    constexpr void Any::Adapt()
    {
        // At this point, the current type is guaranteed not the same as T
        // But size might be the same. If it's not, we need to reallocate the buffer
        const size_t newSize = sizeof(T);
        if (newSize != m_currentTypeSize)
        {
            DeallocateBuffer();

            if constexpr (newSize <= kLocSmallDataSize)
            {
                m_pBuffer = m_smallBuffer;
            }
            else
            {
                m_pBuffer = Allocator<Byte>::AllocateArray(newSize);
            }
        }

        m_constructor = [](Byte* pBuffer, const Byte* pValue)
            {
                Allocator<T>::Construct(reinterpret_cast<T*>(pBuffer), *reinterpret_cast<const T*>(pValue));
            };
        m_destructor = []([[maybe_unused]] Byte* pBuffer)
            {
                if constexpr (!IsTriviallyDestructible<T>)
                {
                    reinterpret_cast<T*>(pBuffer)->~T();
                }
            };
        
        m_currentTypeId   = TypeRegistry::GetId<T>();
        m_currentTypeSize = newSize;
    }
}