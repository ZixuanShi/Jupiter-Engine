// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Box3;

import jpt.Concepts;
import jpt.TVector3;
import jpt.Math;
import jpt.TypeDefs;

export namespace jpt
{
	template<Numeric T>
	struct TBox3
	{
	public:
		TVector3<T> min;
		TVector3<T> max;

	public:
		constexpr TBox3() noexcept = default;
		constexpr TBox3(const TVector3<T>& min, const TVector3<T>& max) noexcept;

		constexpr TVector3<T> Center() const noexcept;
		constexpr TVector3<T> Size() const noexcept;
		constexpr TVector3<T> HalfSize() const noexcept;
		constexpr T Volume() const noexcept;

		constexpr bool Inside(const TVector3<T>& point) const noexcept;
		constexpr bool Intersects(const TBox3<T>& other) const noexcept;

		constexpr bool operator==(const TBox3<T>& other) const noexcept;
	};

	template<Numeric T>
	constexpr TBox3<T>::TBox3(const TVector3<T>& min, const TVector3<T>& max) noexcept
		: min(min)
		, max(max)
	{
	}

	template<Numeric T>
	constexpr TVector3<T> TBox3<T>::Center() const noexcept
	{
		return (min + max) / static_cast<T>(2);
	}

	template<Numeric T>
	constexpr TVector3<T> TBox3<T>::Size() const noexcept
	{
		return max - min;
	}

	template<Numeric T>
	constexpr TVector3<T> TBox3<T>::HalfSize() const noexcept
	{
		return Size() / 2;
	}

	template<Numeric T>
	constexpr T TBox3<T>::Volume() const noexcept
	{
		const TVector3<T> size = Size();
		return size.x * size.y * size.z;
	}

	template<Numeric T>
	constexpr bool TBox3<T>::Inside(const TVector3<T>& point) const noexcept
	{
		return (point.x >= min.x && point.x <= max.x) &&
			   (point.y >= min.y && point.y <= max.y) &&
			   (point.z >= min.z && point.z <= max.z);
	}

	template<Numeric T>
	constexpr bool TBox3<T>::Intersects(const TBox3<T>& other) const noexcept
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
	constexpr bool TBox3<T>::operator==(const TBox3<T>& other) const noexcept
	{
		return min == other.min && max == other.max;
	}
}

export using Box3  = jpt::TBox3<Precision>;
export using Box3f = jpt::TBox3<float32>;
export using Box3i = jpt::TBox3<int32>;