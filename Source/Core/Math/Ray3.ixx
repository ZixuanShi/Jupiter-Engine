// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Ray3;

import jpt.Concepts;
import jpt.Constants;
import jpt.TVector3;
import jpt.TypeDefs;

namespace jpt
{
	export template<Numeric T>
	struct TRay3
	{
	public:
		TVector3<T> origin;
		TVector3<T> direction;

	public:
		constexpr TRay3() noexcept = default;
		constexpr TRay3(const TVector3<T>& origin, const TVector3<T>& direction) noexcept;

		constexpr TVector3<T> GetPoint(T t) const noexcept;
		constexpr T Distance(const TVector3<T>& point) const noexcept;
		constexpr TVector3<T> ClosestPoint(const TVector3<T>& point) const noexcept;
	};

	template<Numeric T>
	constexpr TRay3<T>::TRay3(const TVector3<T>& origin, const TVector3<T>& direction) noexcept
		: origin(origin)
		, direction(direction)
	{
	}

	template<Numeric T>
	constexpr TVector3<T> TRay3<T>::GetPoint(T t) const noexcept
	{
		return origin + direction * t;
	}

	template<Numeric T>
	constexpr T TRay3<T>::Distance(const TVector3<T>& point) const noexcept
	{
		const T dot = TVector3<T>::Dot(point - origin, direction);
		T distance = kInvalidValue<T>;

		// If the point is behind the ray's origin, return the distance to the origin
		if (dot < 0)
		{
			distance = TVector3<T>::Distance(point, origin);
		}
		else
		{
			// Otherwise, return the distance to the point on the ray closest to the point
			const TVector3<T> projection = origin + direction * (dot / direction.Length2());
			distance = TVector3<T>::Distance(point, projection);
		}

		return distance;
	}

	template<Numeric T>
	constexpr TVector3<T> TRay3<T>::ClosestPoint(const TVector3<T>& point) const noexcept
	{
		const TVector3<T> op = point - origin;
		const T t = TVector3<T>::Dot(op, direction);
		if (t <= 0)
		{
			return origin;
		}
		else
		{
			const T d = TVector3<T>::Dot(direction, direction);
			if (t >= d)
			{
				return origin + direction;
			}
			else
			{
				return origin + direction * (t / d);
			}
		}
	}
}

export using Ray3 = jpt::TRay3<Precision>;