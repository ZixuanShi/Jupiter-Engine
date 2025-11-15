// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

export module jpt.Slot;

import jpt.DynamicArray;
import jpt.Function;
import jpt.Math;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Utilities;

export namespace jpt
{
    template<class>
    class Slot;

    /** Array of Functions as Slot */
    template<class TReturn, class... TArgs>
    class Slot<TReturn(TArgs...)>
    {
        using TFunction = Function<TReturn(TArgs...)>;

    private:
        DynamicArray<TFunction> m_functions;

    public:
        /** Add a jpt::Function */
        constexpr Index Add(const TFunction& function);

        /** Add callable object. i.e. global function or local lambda */
        template<class TCallable> requires (!AreSameType<TDecay<TCallable>, Function<TReturn(TArgs...)>>)
        constexpr Index Add(TCallable&& callable);

        /** Add a member function */
        template<class TCaller>
        constexpr Index Add(TCaller* pCaller, TReturn(TCaller::* pMemberFunction)(TArgs...));

        constexpr void Erase(Index index);

        constexpr bool IsConnected(Index index) const;

        constexpr void Clear();

        constexpr void Reserve(Index count);

        constexpr const TFunction& operator[](Index index) const;

        constexpr void operator()(TArgs... args) const;
        constexpr DynamicArray<TReturn> ReturnAll(TArgs... args);
    };

    template<class TReturn, class ...TArgs>
    constexpr Index Slot<TReturn(TArgs...)>::Add(const TFunction& function)
    {
        for (Index i = 0; i < m_functions.Count(); ++i)
        {
            if (!m_functions[i].IsConnected())
            {
                m_functions[i] = function;
                return i;
            }
        }

        m_functions.Add(function);
        return m_functions.Count() - 1;
    }

    template<class TReturn, class ...TArgs>
    template<class TCallable> requires (!AreSameType<TDecay<TCallable>, Function<TReturn(TArgs...)>>)
    constexpr Index Slot<TReturn(TArgs...)>::Add(TCallable&& callable)
    {
        for (Index i = 0; i < m_functions.Count(); ++i)
        {
            if (!m_functions[i].IsConnected())
            {
                m_functions[i].Connect(Forward<TCallable>(callable));
                return i;
            }
        }

        m_functions.EmplaceBack(Forward<TCallable>(callable));
        return m_functions.Count() - 1;
    }

    template<class TReturn, class ...TArgs>
    template<class TCaller>
    constexpr Index Slot<TReturn(TArgs...)>::Add(TCaller* pCaller, TReturn(TCaller::* pMemberFunction)(TArgs...))
    {
        for (Index i = 0; i < m_functions.Count(); ++i)
        {
            if (!m_functions[i].IsConnected())
            {
                m_functions[i].Connect(pCaller, pMemberFunction);
                return i;
            }
        }

        m_functions.EmplaceBack(pCaller, pMemberFunction);
        return m_functions.Count() - 1;
    }

    template<class TReturn, class ...TArgs>
    constexpr void Slot<TReturn(TArgs...)>::Erase(Index index)
    {
        JPT_ASSERT(index < m_functions.Count(), "Index out of range");
        m_functions[index].Disconnect();
    }

    template<class TReturn, class ...TArgs>
    constexpr bool Slot<TReturn(TArgs...)>::IsConnected(Index index) const
    {
        JPT_ASSERT(index < m_functions.Count(), "Index out of range");
        return m_functions[index].IsConnected();
    }

    template<class TReturn, class ...TArgs>
    constexpr void Slot<TReturn(TArgs...)>::Clear()
    {
        m_functions.Clear();
    }

    template<class TReturn, class ...TArgs>
    constexpr void Slot<TReturn(TArgs...)>::Reserve(Index count)
    {
        m_functions.Reserve(count);
    }

    template<class TReturn, class ...TArgs>
    constexpr const Slot<TReturn(TArgs...)>::TFunction& Slot<TReturn(TArgs...)>::operator[](Index index) const
    {
        return m_functions[index];
    }

    template<class TReturn, class ...TArgs>
    constexpr void Slot<TReturn(TArgs...)>::operator()(TArgs ...args) const
    {
        for (const TFunction& function : m_functions)
        {
            if (function.IsConnected())
            {
                function(Forward<TArgs>(args)...);
            }
        }
    }

    template<class TReturn, class ...TArgs>
    constexpr DynamicArray<TReturn> Slot<TReturn(TArgs...)>::ReturnAll(TArgs ...args)
    {
        static_assert(!AreSameType<TReturn, void>, "Can't return from void");

        DynamicArray<TReturn> results;

        for (const TFunction& function : m_functions)
        {
            if (function.IsConnected())
            {
                results.EmplaceBack(function(Forward<TArgs>(args)...));
            }
            else
            {
                results.EmplaceBack();
            }
        }

        return results;
    }
}