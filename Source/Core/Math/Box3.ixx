// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Box3;

import jpt.Concepts;
import jpt.Vector3;
import jpt.Math;
import jpt.TypeDefs;

export namespace jpt
{
	template<Numeric T>
	struct Box3
	{
	public:
		Vector3<T> min;
		Vector3<T> max;

	public:
		constexpr Box3() noexcept = default;
		constexpr Box3(const Vector3<T>& min, const Vector3<T>& max) noexcept;

		constexpr Vector3<T> Center() const noexcept;
		constexpr Vector3<T> Size() const noexcept;
		constexpr Vector3<T> HalfSize() const noexcept;
		constexpr T Volume() const noexcept;

		constexpr bool Inside(const Vector3<T>& point) const noexcept;
		constexpr bool Intersects(const Box3<T>& other) const noexcept;

		constexpr bool operator==(const Box3<T>& other) const noexcept;
	};

	template<Numeric T>
	constexpr Box3<T>::Box3(const Vector3<T>& min, const Vector3<T>& max) noexcept
		: min(min)
		, max(max)
	{
	}

	template<Numeric T>
	constexpr Vector3<T> Box3<T>::Center() const noexcept
	{
		return (min + max) / static_cast<T>(2);
	}

	template<Numeric T>
	constexpr Vector3<T> Box3<T>::Size() const noexcept
	{
		return max - min;
	}

	template<Numeric T>
	constexpr Vector3<T> Box3<T>::HalfSize() const noexcept
	{
		return Size() / 2;
	}

	template<Numeric T>
	constexpr T Box3<T>::Volume() const noexcept
	{
		const Vector3<T> size = Size();
		return size.x * size.y * size.z;
	}

	template<Numeric T>
	constexpr bool Box3<T>::Inside(const Vector3<T>& point) const noexcept
	{
		return (point.x >= min.x && point.x <= max.x) &&
			   (point.y >= min.y && point.y <= max.y) &&
			   (point.z >= min.z && point.z <= max.z);
	}

	template<Numeric T>
	constexpr bool Box3<T>::Intersects(const Box3<T>& other) const noexcept
	{
		if (min.x > other.max.x)
		{
			return false;
		}
		if (max.x < other.min.x)
		{
			return false;
		}
		if (min.y > other.max.y)
		{
			return false;
		}
		if (max.y < other.min.y)
		{
			return false;
		}
		if (min.z > other.max.z)
		{
			return false;
		}
		if (max.z < other.min.z)
		{
			return false;
		}

		return true;
	}

	template<Numeric T>
	constexpr bool Box3<T>::operator==(const Box3<T>& other) const noexcept
	{
		return min == other.min && max == other.max;
	}
}

export using Box3f = jpt::Box3<float32>;
export using Box3i = jpt::Box3<int32>;