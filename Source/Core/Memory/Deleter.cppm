// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

export module jpt_private.Deleter;

import jpt.Allocator;

export namespace jpt_private
{
    template<class T>
    struct DefaultDelete
    {
        /**  Constructs a jpt::DefaultDelete object. */
        constexpr DefaultDelete() noexcept = default;

        /** Constructs a jpt::DefaultDelete<Type> object from another jpt::DefaultDelete object.
            This constructor will only participate in overload resolution if TOtherype* is implicitly convertible to Type* */
        template<class TOther>
        constexpr DefaultDelete(const DefaultDelete<TOther>& deleter) noexcept {}

        template<class TOther>
        constexpr DefaultDelete& operator=(const DefaultDelete<TOther>&) { return *this; }

        constexpr void operator()(T* pPtr) const { jpt::Allocator<T>::Delete(pPtr); }
    };

    template<class T>
    struct DefaultDelete<T[]>
    {
        /**  Constructs a jpt::DefaultDelete object. */
        constexpr DefaultDelete() noexcept = default;

        /** Constructs a jpt::DefaultDelete<Type> object from another jpt::DefaultDelete object.
            This constructor will only participate in overload resolution if TOtherype* is implicitly convertible to Type* */
        template<class TOther>
        constexpr DefaultDelete(const DefaultDelete<TOther[]>& deleter) noexcept {}

        template<class TOther>
        DefaultDelete& operator=(const DefaultDelete<TOther[]>&) { return *this; }

        constexpr void operator()(T* pPtr) const { delete[] pPtr; }
    };
}