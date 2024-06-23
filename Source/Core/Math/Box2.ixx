// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Box2;

import jpt.Concepts;
import jpt.Vector2;
import jpt.Math;
import jpt.TypeDefs;

export namespace jpt
{
	template<Numeric T>
	struct Box2
	{
	public:
		Vector2<T> min;
		Vector2<T> max;

	public:
		constexpr Box2() noexcept = default;
		constexpr Box2(const Vector2<T>& min, const Vector2<T>& max) noexcept;

		constexpr Vector2<T> Center() const noexcept;
		constexpr Vector2<T> Size() const noexcept;
		constexpr Vector2<T> HalfSize() const noexcept;

		constexpr bool Inside(const Vector2<T>& point) const noexcept;
		constexpr bool Intersects(const Box2<T>& other) const noexcept;

		constexpr bool operator==(const Box2<T>& other) const noexcept;
	};

	template<Numeric T>
	constexpr Box2<T>::Box2(const Vector2<T>& min, const Vector2<T>& max) noexcept
		: min(min)
		, max(max)
	{
	}

	template<Numeric T>
	constexpr Vector2<T> Box2<T>::Center() const noexcept
	{
		return (min + max) / static_cast<T>(2);
	}

	template<Numeric T>
	constexpr Vector2<T> Box2<T>::Size() const noexcept
	{
		return max - min;
	}

	template<Numeric T>
	constexpr Vector2<T> Box2<T>::HalfSize() const noexcept
	{
		return Size() / 2;
	}

	template<Numeric T>
	constexpr bool Box2<T>::Inside(const Vector2<T>& point) const noexcept
	{
		return (point.x >= min.x && point.x <= max.x) && 
			   (point.y >= min.y && point.y <= max.y);
	}

	template<Numeric T>
	constexpr bool Box2<T>::Intersects(const Box2<T>& other) const noexcept
	{
		return (min.x <= other.max.x && max.x >= other.min.x) && 
			   (min.y <= other.max.y && max.y >= other.min.y);
	}

	template<Numeric T>
	constexpr bool Box2<T>::operator==(const Box2<T>& other) const noexcept
	{
		return min == other.min && max == other.max;
	}
}

export using Box2f = jpt::Box2<float32>;
export using Box2i = jpt::Box2<int32>;