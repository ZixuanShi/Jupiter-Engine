// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <cmath>
#include <type_traits>

export module jpt.TVector3;

import jpt.Concepts;
import jpt.TypeDefs;
import jpt.TypeTraits;
import jpt.Math;
import jpt.String;
import jpt.ToString;
import jpt.Hash;

export namespace jpt
{
	template<Numeric T>
	struct TVector3
	{
	public:
		T x = static_cast<T>(0);
		T y = static_cast<T>(0);
		T z = static_cast<T>(0);

	public:
		using NumericType = T;

		static consteval TVector3 Zero() { return TVector3( static_cast<T>(0),  static_cast<T>(0),  static_cast<T>(0)); }
		static consteval TVector3 One()  { return TVector3( static_cast<T>(1),  static_cast<T>(1),  static_cast<T>(1)); }

		// Right-handed
		// X. Pitch
		static consteval TVector3 Left()  { return TVector3(static_cast<T>(-1), static_cast<T>(0), static_cast<T>(0)); }
		static consteval TVector3 Right() { return TVector3(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)); }

		// Y. Yaw
		static consteval TVector3 Up()   { return TVector3( static_cast<T>(0),  static_cast<T>(1),  static_cast<T>(0)); }
		static consteval TVector3 Down() { return TVector3( static_cast<T>(0), static_cast<T>(-1),  static_cast<T>(0)); }

		// Z. Roll
		static consteval TVector3 Forward()  { return TVector3( static_cast<T>(0),  static_cast<T>(0),  static_cast<T>(1)); }
		static consteval TVector3 Backward() { return TVector3( static_cast<T>(0),  static_cast<T>(0), static_cast<T>(-1)); }

	public:
		constexpr TVector3() = default;
		constexpr TVector3(T scalar);
		constexpr TVector3(T _x, T _y, T _z);

		constexpr TVector3 operator-() const;

		constexpr TVector3 operator+(const TVector3& other) const;
		constexpr TVector3 operator-(const TVector3& other) const;
		constexpr TVector3 operator*(const TVector3& other) const;
		constexpr TVector3 operator/(const TVector3& other) const;

		constexpr TVector3& operator+=(const TVector3& other);
		constexpr TVector3& operator-=(const TVector3& other);
		constexpr TVector3& operator*=(const TVector3& other);
		constexpr TVector3& operator/=(const TVector3& other);

		constexpr TVector3 operator+(T scalar) const;
		constexpr TVector3 operator-(T scalar) const;
		constexpr TVector3 operator*(T scalar) const;
		constexpr TVector3 operator/(T scalar) const;

		constexpr TVector3& operator+=(T scalar);
		constexpr TVector3& operator-=(T scalar);
		constexpr TVector3& operator*=(T scalar);
		constexpr TVector3& operator/=(T scalar);

		constexpr T& operator[](size_t index) { return (&x)[index]; }
		constexpr const T& operator[](size_t index) const { return (&x)[index]; }

		constexpr bool operator==(const TVector3& other) const;

		constexpr T Dot(const TVector3& other) const;
		constexpr TVector3 Cross(const TVector3& other) const;
		constexpr T Length() const;
		constexpr T Length2() const;
		constexpr T Distance(const TVector3& other) const;
		constexpr T Distance2(const TVector3& other) const;
		constexpr void Normalize();
		constexpr TVector3 Normalized() const;
		constexpr T Angle(const TVector3& other) const;
		constexpr TVector3 Lerp(const TVector3& other, T t) const;
		constexpr TVector3 InvLerp(const TVector3& other, const TVector3& value) const;
		constexpr TVector3 RotateAxis(const TVector3& axis, T radians) const;
		constexpr TVector3 RotatePoint(const TVector3& point, const TVector3& axis, T radians) const;
		constexpr TVector3 Project(const TVector3& to) const;
		constexpr bool OnLeft(const TVector3& viewPosition, const TVector3& viewDirection) const;

		constexpr static T Dot(const TVector3& left, const TVector3& right);
		constexpr static TVector3 Cross(const TVector3& left, const TVector3& right);
		constexpr static T Length(const TVector3& TVector3);
		constexpr static T Length2(const TVector3& TVector3);
		constexpr static T Distance(const TVector3& left, const TVector3& right);
		constexpr static T Distance2(const TVector3& left, const TVector3& right);
		constexpr static TVector3 Normalize(const TVector3& TVector3);
		constexpr static T Angle(const TVector3& left, const TVector3& right);
		constexpr static TVector3 Lerp(const TVector3& start, const TVector3& end, T t);
		constexpr static TVector3 InvLerp(const TVector3& start, const TVector3& end, const TVector3& value);
		constexpr static TVector3 Project(const TVector3& from, const TVector3& to);

		constexpr String ToString() const;
		constexpr uint64 Hash() const;
	};

	// ------------------------------------------------------------------------------------------------
	// Member functions
	// ------------------------------------------------------------------------------------------------
	template<Numeric T>
	constexpr TVector3<T>::TVector3(T scalar)
		: x(scalar)
		, y(scalar)
		, z(scalar)
	{
	}

	template<Numeric T>
	constexpr TVector3<T>::TVector3(T _x, T _y, T _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::operator-() const
	{
		return TVector3(-x, -y, -z);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::operator+(const TVector3& other) const
	{
		return TVector3(x + other.x, y + other.y, z + other.z);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::operator-(const TVector3& other) const
	{
		return TVector3(x - other.x, y - other.y, z - other.z);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::operator*(const TVector3& other) const
	{
		return TVector3(x * other.x, y * other.y, z * other.z);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::operator/(const TVector3& other) const
	{
		return TVector3(x / other.x, y / other.y, z / other.z);
	}

	template<Numeric T>
	constexpr TVector3<T>& TVector3<T>::operator+=(const TVector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	template<Numeric T>
	constexpr TVector3<T>& TVector3<T>::operator-=(const TVector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template<Numeric T>
	constexpr TVector3<T>& TVector3<T>::operator*=(const TVector3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	template<Numeric T>
	constexpr TVector3<T>& TVector3<T>::operator/=(const TVector3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::operator+(T scalar) const
	{
		return TVector3(x + scalar, y + scalar, z + scalar);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::operator-(T scalar) const
	{
		return TVector3(x - scalar, y - scalar, z - scalar);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::operator*(T scalar) const
	{
		return TVector3(x * scalar, y * scalar, z * scalar);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::operator/(T scalar) const
	{
		return TVector3(x / scalar, y / scalar, z / scalar);
	}

	template<Numeric T>
	constexpr TVector3<T>& TVector3<T>::operator+=(T scalar)
	{
		x += scalar;
		y += scalar;
		z += scalar;
		return *this;
	}

	template<Numeric T>
	constexpr TVector3<T>& TVector3<T>::operator-=(T scalar)
	{
		x -= scalar;
		y -= scalar;
		z -= scalar;
		return *this;
	}

	template<Numeric T>
	constexpr TVector3<T>& TVector3<T>::operator*=(T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	template<Numeric T>
	constexpr TVector3<T>& TVector3<T>::operator/=(T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	template<Numeric T>
	constexpr bool TVector3<T>::operator==(const TVector3& other) const
	{
		return AreValuesClose(x, other.x, static_cast<T>(0.05)) && 
			   AreValuesClose(y, other.y, static_cast<T>(0.05)) && 
			   AreValuesClose(z, other.z, static_cast<T>(0.05));
	}

	template<Numeric T>
	constexpr T TVector3<T>::Dot(const TVector3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::Cross(const TVector3& other) const
	{
		return TVector3(y * other.z - z * other.y, 
			           z * other.x - x * other.z, 
			           x * other.y - y * other.x);
	}

	template<Numeric T>
	constexpr T TVector3<T>::Length() const
	{
		return std::sqrt(Length2());
	}

	template<Numeric T>
	constexpr T TVector3<T>::Length2() const
	{
		return x * x + y * y + z * z;
	}

	template<Numeric T>
	constexpr T TVector3<T>::Distance(const TVector3& other) const
	{
		return std::sqrt(Distance2(other));
	}

	template<Numeric T>
	constexpr T TVector3<T>::Distance2(const TVector3& other) const
	{
		return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z);
	}

	template<Numeric T>
	constexpr void TVector3<T>::Normalize()
	{
		const T length = Length();
		if (length > static_cast<T>(0))
		{
			x /= length;
			y /= length;
			z /= length;
		}
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::Normalized() const
	{
		TVector3<T> result = *this;
		result.Normalize();
		return result;
	}

	template<Numeric T>
	constexpr T TVector3<T>::Angle(const TVector3& other) const
	{
		return std::acos(Dot(other) / (Length() * other.Length()));
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::Lerp(const TVector3& other, T t) const
	{
		return *this + (other - *this) * t;
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::InvLerp(const TVector3& other, const TVector3&value) const
	{
		return (value - *this) / (other - *this);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::RotateAxis(const TVector3& axis, T radians) const
	{
		// Rotates a position vector around an axis by a given angle
		// https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula

		const T cosTheta = std::cos(radians);
		const T sinTheta = std::sin(radians);

		const TVector3<T> cross = axis.Cross(*this);
		const TVector3<T> crossCross = axis.Cross(cross);

		return *this * cosTheta + cross * sinTheta + crossCross * (static_cast<T>(1) - cosTheta);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::RotatePoint(const TVector3& point, const TVector3& axis, T radians) const
	{
		// Rotates this position vector around a point by a given angle around a specified axis
		const TVector3<T> offset = *this - point;
		const TVector3<T> rotated = offset.RotateAxis(axis, radians);
		return rotated + point;
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::Project(const TVector3& to) const
	{
		// Projects from this vector to the other
		// Formula: proj_v(u) = (u . v / |v|^2) * v

		const T length2 = to.Length2();
		if (length2 == static_cast<T>(0))
		{
			return TVector3<T>::Zero();
		}

		const T dot = this->Dot(to);
		const T scalar = dot / length2;
		return to * scalar;
	}

	template<Numeric T>
	constexpr bool TVector3<T>::OnLeft(const TVector3& viewPosition, const TVector3& viewDirection) const
	{
		// Calculate the vector from the view position to the point
		const TVector3 viewToPoint = *this - viewPosition;

		// Calculate the cross product of the view direction and the view to point vector
		const TVector3 cross = viewDirection.Cross(viewToPoint);

		// If the cross product is pointing up, the point is to the left of the view
		return cross.y > static_cast<T>(0);
	}

	template<Numeric T>
	constexpr T TVector3<T>::Dot(const TVector3& left, const TVector3& right)
	{
		return left.Dot(right);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::Cross(const TVector3& left, const TVector3& right)
	{
		return left.Cross(right);
	}

	template<Numeric T>
	constexpr T TVector3<T>::Length(const TVector3& TVector3)
	{
		return TVector3.Length();
	}

	template<Numeric T>
	constexpr T TVector3<T>::Length2(const TVector3& TVector3)
	{
		return TVector3.Length2();
	}

	template<Numeric T>
	constexpr T TVector3<T>::Distance(const TVector3& left, const TVector3& right)
	{
		return left.Distance(right);
	}

	template<Numeric T>
	constexpr T TVector3<T>::Distance2(const TVector3& left, const TVector3& right)
	{
		return left.Distance2(right);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::Normalize(const TVector3& vector)
	{
		TVector3<T> result = vector;
		result.Normalize();
		return result;
	}

	template<Numeric T>
	constexpr T TVector3<T>::Angle(const TVector3& left, const TVector3& right)
	{
		return left.Angle(right);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::Lerp(const TVector3& start, const TVector3& end, T t)
	{
		return start.Lerp(end, t);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::InvLerp(const TVector3& start, const TVector3& end, const TVector3& value)
	{
		return start.InvLerp(end, value);
	}

	template<Numeric T>
	constexpr TVector3<T> TVector3<T>::Project(const TVector3& from, const TVector3& to)
	{
		return from.Project(to);
	}

	template<Numeric T>
	constexpr String TVector3<T>::ToString() const
	{
		return String::Format<64>("x: %.3f, y: %.3f, z: %.3f", x, y, z);
	}

	template<Numeric T>
	constexpr uint64 TVector3<T>::Hash() const
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			const T epsilon = static_cast<T>(1e-6);
			auto round = [epsilon](T value) -> T
				{
					return (value < epsilon && value > -epsilon) ? static_cast<T>(0) : value;
				};

			uint64 hash = jpt::Hash(round(x));
			hash ^= jpt::Hash(round(y)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= jpt::Hash(round(z)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			return hash;
		}
		else
		{
			uint64 hash = jpt::Hash(x);
			hash ^= jpt::Hash(y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= jpt::Hash(z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			return hash;
		}
	}
}

export using Vec3  = jpt::TVector3<Precision>;
export using Vec3f = jpt::TVector3<float>;
export using Vec3d = jpt::TVector3<double>;
export using Vec3i = jpt::TVector3<int32>;
export using Vec3u = jpt::TVector3<uint32>;

template<> constexpr bool jpt::IsTrivial<Vec3f> = true;
template<> constexpr bool jpt::IsTrivial<Vec3i> = true;
template<> constexpr bool jpt::IsTrivial<Vec3u> = true;

export template<jpt::Numeric T>
constexpr jpt::TVector3<T> operator+(T scaler, const jpt::TVector3<T>& TVector3)
{
	return TVector3 + scaler;
}

export template<jpt::Numeric T>
constexpr jpt::TVector3<T> operator-(T scaler, const jpt::TVector3<T>& TVector3)
{
	return TVector3 - scaler;
}

export template<jpt::Numeric T>
constexpr jpt::TVector3<T> operator*(T scaler, const jpt::TVector3<T>& TVector3)
{
	return TVector3 * scaler;
}

export template<jpt::Numeric T>
constexpr jpt::TVector3<T> operator/(T scaler, const jpt::TVector3<T>& TVector3)
{
	return TVector3 / scaler;
}