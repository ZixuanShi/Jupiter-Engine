// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.Deleter;

export namespace jpt_private
{
	template<class T>
	struct DefaultDelete
	{
		/**  Constructs a jpt::DefaultDelete object. */
		constexpr DefaultDelete() noexcept = default;

		/** Constructs a jpt::DefaultDelete<Type> object from another jpt::DefaultDelete object.
			This constructor will only participate in overload resolution if OtherType* is implicitly convertible to Type* */
		template<class OtherT>
		constexpr DefaultDelete(const DefaultDelete<OtherT>& deleter) noexcept {}

		template<class OtherT>
		constexpr DefaultDelete& operator=(const DefaultDelete<OtherT>&) { return *this; }

		constexpr void operator()(T* pPtr) const { delete pPtr; }
	};

	template<class T>
	struct DefaultDelete<T[]>
	{
		/**  Constructs a jpt::DefaultDelete object. */
		constexpr DefaultDelete() noexcept = default;

		/** Constructs a jpt::DefaultDelete<Type> object from another jpt::DefaultDelete object.
			This constructor will only participate in overload resolution if OtherType* is implicitly convertible to Type* */
		template<class OtherT>
		constexpr DefaultDelete(const DefaultDelete<OtherT[]>& deleter) noexcept {}

		template<class OtherT>
		DefaultDelete& operator=(const DefaultDelete<OtherT[]>&) { return *this; }

		constexpr void operator()(T* pPtr) const { delete[] pPtr; }
	};
}