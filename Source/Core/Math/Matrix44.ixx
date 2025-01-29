// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Validation/Assert.h"

#include <cmath>

export module jpt.Matrix44;

import jpt.Concepts;
import jpt.Constants;
import jpt.TypeDefs;
import jpt.Math;
import jpt.Math.Settings;
import jpt.TVector3;
import jpt.Vector4;
import jpt.Utilities;
import jpt.String;
import jpt.ToString;

namespace jpt
{
	export template<Numeric T>
	struct TMatrix44
	{
	public:
		Vector4<T> m[4];

	public:
		using NumericType = T;

		static consteval TMatrix44 Identity() { return TMatrix44(); }
		static consteval TMatrix44 Zero() { return TMatrix44(Vector4<T>::Zero(), Vector4<T>::Zero(), Vector4<T>::Zero(), Vector4<T>::Zero()); }

	public:
		constexpr TMatrix44();
		constexpr TMatrix44(const Vector4<T>& v0, const Vector4<T>& v1, const Vector4<T>& v2, const Vector4<T>& v3);
		constexpr TMatrix44(T m00, T m01, T m02, T m03,
		                   T m10, T m11, T m12, T m13,
		                   T m20, T m21, T m22, T m23,
		                   T m30, T m31, T m32, T m33);

		constexpr TMatrix44<T> operator*(const TMatrix44<T>& rhs) const;
		constexpr TVector3<T> operator*(const TVector3<T>& rhs) const;
		constexpr Vector4<T> operator*(const Vector4<T>& rhs) const;
		constexpr TMatrix44<T>& operator*=(const TMatrix44<T>& rhs);

		constexpr T Determinant() const;

		constexpr static TMatrix44<T> Translation(const TVector3<T>& v);
		constexpr static TMatrix44<T> Translation(T x, T y, T z);
		constexpr static TMatrix44<T> RotationX(T radians);
		constexpr static TMatrix44<T> RotationY(T radians);
		constexpr static TMatrix44<T> RotationZ(T radians);
		constexpr static TMatrix44<T> Scaling(const TVector3<T>& v);
		constexpr static TMatrix44<T> Scaling(T x, T y, T z);
		constexpr static TMatrix44<T> Transposed(const TMatrix44<T>& matrix44);

		/** Creates an orthographic projection matrix */
		constexpr static TMatrix44<T> Orthographic(T left, T right, T bottom, T top, T near, T far);

		/** Creates a view matrix for a camera, defining how the world is oriented relative to the camera's position */
		constexpr static TMatrix44<T> LookAt(const TVector3<T>& eye, const TVector3<T>& center, const TVector3<T>& up);

		/** Converts 3D coordinates into 2D screen coordinates */
		constexpr static TMatrix44<T> Perspective(T fov, T aspect, T near, T far);

		constexpr void Translate(const TVector3<T>& v);
		constexpr void Translate(T x, T y, T z);
		constexpr void RotateX(T radians);
		constexpr void RotateY(T radians);
		constexpr void RotateZ(T radians);
		constexpr void Scale(const TVector3<T>& v);
		constexpr void Scale(T x, T y, T z);
		constexpr void Transpose();
		constexpr void Inverse();
		constexpr bool IsOrthogonal() const;

		// Euler Conversion
		constexpr TVector3<T> ToEulerDegrees() const;
		constexpr TVector3<T> ToEulerRadians() const;
		constexpr static TMatrix44<T> FromDegrees(const TVector3<T>& degrees);
		constexpr static TMatrix44<T> FromDegrees(T pitch, T yaw, T roll);
		constexpr static TMatrix44<T> FromRadians(const TVector3<T>& radians);
		constexpr static TMatrix44<T> FromRadians(T pitch, T yaw, T roll);

		// Accessors
		constexpr       Vector4<T>& operator[](size_t index)       { return m[index]; }
		constexpr const Vector4<T>& operator[](size_t index) const { return m[index]; }

		constexpr String ToString() const;
		constexpr bool operator==(const TMatrix44<T>& rhs) const;
	};

	template<Numeric T>
	constexpr TMatrix44<T>::TMatrix44()
		: m{ { 1, 0, 0, 0 }, 
		     { 0, 1, 0, 0 }, 
		     { 0, 0, 1, 0 }, 
		     { 0, 0, 0, 1 } }
	{
	}

	template<Numeric T>
	constexpr TMatrix44<T>::TMatrix44(const Vector4<T>& v0, const Vector4<T>& v1, const Vector4<T>& v2, const Vector4<T>& v3)
		: m{ v0, v1, v2, v3 }
	{
	}
	template<Numeric T>
	constexpr TMatrix44<T>::TMatrix44(T m00, T m01, T m02, T m03, 
		                            T m10, T m11, T m12, T m13, 
		                            T m20, T m21, T m22, T m23, 
		                            T m30, T m31, T m32, T m33)
		: m{ { m00, m01, m02, m03 },
			 { m10, m11, m12, m13 },
			 { m20, m21, m22, m23 },
			 { m30, m31, m32, m33 } }
	{
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::operator*(const TMatrix44<T>& rhs) const
	{
		TMatrix44<T> result;

		for (size_t i = 0; i < 4; ++i)
		{
			for (size_t j = 0; j < 4; ++j)
			{
				result.m[j][i] = m[0][i] * rhs.m[j][0] + 
				                 m[1][i] * rhs.m[j][1] + 
				                 m[2][i] * rhs.m[j][2] + 
				                 m[3][i] * rhs.m[j][3];
			}
		}

		return result;
	}

	template<Numeric T>
	constexpr TVector3<T> TMatrix44<T>::operator*(const TVector3<T>& rhs) const
	{
		Vector4<T> result = *this * Vector4<T>(rhs, 1);
		return TVector3<T>(result.x, result.y, result.z);
	}

	template<Numeric T>
	constexpr Vector4<T> TMatrix44<T>::operator*(const Vector4<T>& rhs) const
	{
		Vector4<T> result;
		for (size_t i = 0; i < 4; ++i)
		{
			result[i] = m[0][i] * rhs[0] + 
			            m[1][i] * rhs[1] + 
			            m[2][i] * rhs[2] + 
			            m[3][i] * rhs[3];
		}
		return result;
	}

	template<Numeric T>
	constexpr TMatrix44<T>& TMatrix44<T>::operator*=(const TMatrix44<T>& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	template<Numeric T>
	constexpr T TMatrix44<T>::Determinant() const
	{
		const T a = m[0][0];
		const T b = m[0][1];
		const T c = m[0][2];
		const T d = m[0][3];
		const T e = m[1][0];
		const T f = m[1][1];
		const T g = m[1][2];
		const T h = m[1][3];
		const T i = m[2][0];
		const T j = m[2][1];
		const T k = m[2][2];
		const T l = m[2][3];
		const T m = m[3][0];
		const T n = m[3][1];
		const T o = m[3][2];
		const T p = m[3][3];

		const T det = a * f * k * p + a * g * l * n + a * h * j * o +
					  b * e * l * p + b * g * i * p + b * h * k * n +
					  c * e * j * p + c * f * l * m + c * h * i * m +
					  d * e * k * n + d * f * i * o + d * g * j * m -
					  a * f * l * o - a * g * j * p - a * h * k * m -
					  b * e * k * o - b * g * l * m - b * h * i * p -
					  c * e * l * n - c * f * i * p - c * h * j * m -
					  d * e * j * n - d * f * k * m - d * g * i * l;

		return det;
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Translation(const TVector3<T>& v)
	{
		return TMatrix44<T>(  1,   0,   0, 0,
			                  0,   1,   0, 0,
			                  0,   0,   1, 0,
			                v.x, v.y, v.z, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Translation(T x, T y, T z)
	{
		return TMatrix44<T>(1, 0, 0, 0,
						    0, 1, 0, 0,
						    0, 0, 1, 0,
						    x, y, z, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::RotationX(T radians)
	{
		const T cos = std::cos(radians);
		const T sin = std::sin(radians);
		return TMatrix44<T>(1,   0,    0, 0,
			               0, cos, -sin, 0,
			               0, sin,  cos, 0,
			               0,   0,    0, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::RotationY(T radians)
	{
		const T cos = std::cos(radians);
		const T sin = std::sin(radians);
		return TMatrix44<T>( cos, 0, sin, 0,
			                  0, 1,   0, 0,
			               -sin, 0, cos, 0,
			                  0, 0,   0, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::RotationZ(T radians)
	{
		const T cos = std::cos(radians);
		const T sin = std::sin(radians);
		return TMatrix44<T>(cos, -sin, 0, 0,
			               sin,  cos, 0, 0,
			                 0,    0, 1, 0,
			                 0,    0, 0, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Scaling(const TVector3<T>& v)
	{
		return TMatrix44<T>(v.x,   0,   0, 0,
			                 0, v.y,   0, 0,
			                 0,   0, v.z, 0,
			                 0,   0,   0, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Scaling(T x, T y, T z)
	{
		return TMatrix44<T>(x, 0, 0, 0,
							0, y, 0, 0,
							0, 0, z, 0,
							0, 0, 0, 1);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Transposed(const TMatrix44<T>& matrix44)
	{
		TMatrix44<T> result = matrix44;
		result.Transpose();
		return result;
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Orthographic(T left, T right, T bottom, T top, T near, T far)
	{
		TMatrix44<T> result = TMatrix44<T>::Identity();

		const T width = right - left;
		const T height = top - bottom;
		const T depth = far - near;

		result.m[0][0] = static_cast<T>(2) / width;
		result.m[1][1] = static_cast<T>(2) / height;
		result.m[2][2] = -static_cast<T>(2) / depth;
		result.m[3][0] = -(right + left) / width;
		result.m[3][1] = -(top + bottom) / height;
		result.m[3][2] = -(far + near) / depth;

		return result;
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::LookAt(const TVector3<T>& eye, const TVector3<T>& center, const TVector3<T>& up)
	{
		// Calcualte the forward vector
		const TVector3<T> forward = (center - eye).Normalized();

		// Calculate the right vector
		const TVector3<T> right = forward.Cross(up).Normalized();

		// Calculate the up vector
		const TVector3<T> newUp = right.Cross(forward);

		// Create the view matrix
		TMatrix44<T> result = TMatrix44<T>::Identity();
		result.m[0][0] = right.x;
		result.m[1][0] = right.y;
		result.m[2][0] = right.z;

		result.m[0][1] = newUp.x;
		result.m[1][1] = newUp.y;
		result.m[2][1] = newUp.z;

		result.m[0][2] = -forward.x;
		result.m[1][2] = -forward.y;
		result.m[2][2] = -forward.z;

		result.m[3][0] = -right.Dot(eye);
		result.m[3][1] = -newUp.Dot(eye);
		result.m[3][2] = forward.Dot(eye);

		return result;
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::Perspective(T fov, T aspect, T zNear, T zFar)
	{
		const T tanHalfFovy = std::tan(fov / static_cast<T>(2));
		TMatrix44<T> result = TMatrix44<T>::Zero();

		result.m[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
		result.m[1][1] = static_cast<T>(1) / tanHalfFovy;
		result.m[2][2] = -(zFar + zNear) / (zFar - zNear);
		result.m[2][3] = -static_cast<T>(1);
		result.m[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);

		return result;
	}

	template<Numeric T>
	constexpr TVector3<T> TMatrix44<T>::ToEulerDegrees() const
	{
		return ToDegrees(ToEulerRadians());
	}

	template<Numeric T>
	constexpr TVector3<T> TMatrix44<T>::ToEulerRadians() const
	{
		TVector3<T> euler;

		switch (MathSettings::RotationOrder)
		{
			case RotationOrder::XYZ:
			{
				euler.x = std::asin(Clamp(m[1][2], static_cast<T>(-1), static_cast<T>(1)));  // Pitch

				if (std::abs(m[1][2]) < static_cast<T>(0.9999999))
				{
					euler.y = std::atan2(-m[0][2], m[2][2]);    // Yaw
					euler.z = std::atan2(-m[1][0], m[1][1]);    // Roll
				}
				else
				{
					euler.y = std::atan2(m[2][0], m[0][0]);
					euler.z = 0;
				}
				break;
			}
			case RotationOrder::XZY:
			{
				euler.x = std::asin(Clamp(-m[1][2], static_cast<T>(-1), static_cast<T>(1)));  // Pitch

				if (std::abs(m[1][2]) < static_cast<T>(0.9999999))
				{
					euler.y = std::atan2(m[0][2], m[2][2]);     // Yaw
					euler.z = std::atan2(m[1][0], m[1][1]);     // Roll
				}
				else
				{
					euler.y = std::atan2(-m[2][0], m[0][0]);
					euler.z = 0;
				}
				break;
			}
			[[likely]] case RotationOrder::YXZ:
			{
				euler.x = std::asin(Clamp(-m[1][2], static_cast<T>(-1), static_cast<T>(1)));  // Pitch

				if (std::abs(m[1][2]) < static_cast<T>(0.9999999))
				{
					euler.y = std::atan2(m[0][2], m[2][2]);     // Yaw
					euler.z = std::atan2(m[1][0], m[1][1]);     // Roll
				}
				else
				{
					euler.y = std::atan2(-m[2][0], m[0][0]);
					euler.z = 0;
				}
				break;
			}
			case RotationOrder::YZX:
			{
				euler.x = std::asin(Clamp(m[2][1], static_cast<T>(-1), static_cast<T>(1)));  // Pitch

				if (std::abs(m[2][1]) < static_cast<T>(0.9999999))
				{
					euler.y = std::atan2(-m[2][0], m[2][2]);    // Yaw
					euler.z = std::atan2(-m[0][1], m[1][1]);    // Roll
				}
				else
				{
					euler.y = std::atan2(m[0][2], m[0][0]);
					euler.z = 0;
				}
				break;
			}
			case RotationOrder::ZXY:
			{
				euler.x = std::asin(Clamp(m[1][2], static_cast<T>(-1), static_cast<T>(1)));  // Pitch

				if (std::abs(m[1][2]) < static_cast<T>(0.9999999))
				{
					euler.y = std::atan2(-m[0][2], m[2][2]);    // Yaw
					euler.z = std::atan2(-m[1][0], m[1][1]);    // Roll
				}
				else
				{
					euler.y = std::atan2(m[2][0], m[0][0]);
					euler.z = 0;
				}
				break;
			}
			case RotationOrder::ZYX:
			{
				euler.x = std::asin(Clamp(-m[2][1], static_cast<T>(-1), static_cast<T>(1)));  // Pitch

				if (std::abs(m[2][1]) < static_cast<T>(0.9999999))
				{
					euler.y = std::atan2(m[2][0], m[2][2]);     // Yaw
					euler.z = std::atan2(m[0][1], m[1][1]);     // Roll
				}
				else
				{
					euler.y = std::atan2(-m[0][2], m[0][0]);
					euler.z = 0;
				}
				break;
			}
			default:
			{
				JPT_ASSERT(false, "Invalid Rotation Order");
				return TVector3<T>();
			}
		}

		return euler;
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::FromDegrees(const TVector3<T>& degrees)
	{
		const TVector3<T> radians = ToRadians(degrees);
		return FromRadians(radians);
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::FromDegrees(T pitch, T yaw, T roll)
	{
		return FromDegrees(TVector3<T>(pitch, yaw, roll));
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::FromRadians(const TVector3<T>& radians)
	{
		// Calculate the cosine and sine of the angles
		const T cx = std::cos(radians.x);  // pitch
		const T cy = std::cos(radians.y);  // yaw
		const T cz = std::cos(radians.z);  // roll
		const T sx = std::sin(radians.x);
		const T sy = std::sin(radians.y);
		const T sz = std::sin(radians.z);

		// Apply rotation based on order
		switch (MathSettings::RotationOrder)
		{
			case RotationOrder::XZY:
			{
				const T m00 = cy * cz;
				const T m01 = -cy * sz * cx + sy * sx;
				const T m02 = cy * sz * sx + sy * cx;
				const T m10 = sz;
				const T m11 = cx * cz;
				const T m12 = -sx * cz;
				const T m20 = -sy * cz;
				const T m21 = sy * sz * cx + cy * sx;
				const T m22 = -sy * sz * sx + cy * cx;
				return TMatrix44<T>(m00, m01, m02, 0,
					               m10, m11, m12, 0,
					               m20, m21, m22, 0,
					                 0,   0,   0, 1);
			}
			case RotationOrder::XYZ:
			{
				const T m00 = cy * cz;
				const T m01 = -cy * sz;
				const T m02 = sy;
				const T m10 = sx * sy * cz + cx * sz;
				const T m11 = -sx * sy * sz + cx * cz;
				const T m12 = -sx * cy;
				const T m20 = -cx * sy * cz + sx * sz;
				const T m21 = cx * sy * sz + sx * cz;
				const T m22 = cx * cy;
				return TMatrix44<T>(m00, m01, m02, 0,
					               m10, m11, m12, 0,
					               m20, m21, m22, 0,
					                 0,   0,   0, 1);
			}
			[[likely]] case RotationOrder::YXZ:
			{
				const T m00 = cy * cz + sy * sx * sz;
				const T m01 = -cy * sz + sy * sx * cz;
				const T m02 = sy * cx;
				const T m10 = cx * sz;
				const T m11 = cx * cz;
				const T m12 = -sx;
				const T m20 = -sy * cz + cy * sx * sz;
				const T m21 = sy * sz + cy * sx * cz;
				const T m22 = cy * cx;
				return TMatrix44<T>(m00, m01, m02, 0,
					               m10, m11, m12, 0,
					               m20, m21, m22, 0,
					                 0,   0,   0, 1);
			}
			case RotationOrder::YZX:
			{
				const T m00 = cy * cz;
				const T m01 = -sz;
				const T m02 = sy;
				const T m10 = cx * cy * sz + sx * sy;
				const T m11 = cx * cz;
				const T m12 = -cx * sy * sz + sx * cy;
				const T m20 = sx * cy * sz - cx * sy;
				const T m21 = sx * cz;
				const T m22 = -sx * sy * sz - cx * cy;
				return TMatrix44<T>(m00, m01, m02, 0,
					               m10, m11, m12, 0,
					               m20, m21, m22, 0,
					                 0,   0,   0, 1);
			}
			case RotationOrder::ZXY:
			{
				const T m00 = cy * cz - sy * sx * sz;
				const T m01 = -cx * sz;
				const T m02 = sy * cz + cy * sx * sz;
				const T m10 = cy * sz + sy * sx * cz;
				const T m11 = cx * cz;
				const T m12 = sy * sz - cy * sx * cz;
				const T m20 = -sy * cx;
				const T m21 = sx;
				const T m22 = cy * cx;
				return TMatrix44<T>(m00, m01, m02, 0,
					               m10, m11, m12, 0,
					               m20, m21, m22, 0,
					                 0,   0,   0, 1);
			}
			case RotationOrder::ZYX:
			{
				const T m00 = cy * cz;
				const T m01 = -sz;
				const T m02 = sy * cz;
				const T m10 = cy * sz * cx + sy * sx;
				const T m11 = cz * cx;
				const T m12 = sy * sz * cx - cy * sx;
				const T m20 = cy * sz * sx - sy * cx;
				const T m21 = cz * sx;
				const T m22 = sy * sz * sx + cy * cx;
				return TMatrix44<T>(m00, m01, m02, 0,
					               m10, m11, m12, 0,
					               m20, m21, m22, 0,
					                 0,   0,   0, 1);
			}
			default:
			{
				JPT_ASSERT(false, "Invalid Rotation Order");
				return TMatrix44<T>();
			}
		}
	}

	template<Numeric T>
	constexpr TMatrix44<T> TMatrix44<T>::FromRadians(T pitch, T yaw, T roll)
	{
		return FromRadians(TVector3<T>(pitch, yaw, roll));
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Translate(const TVector3<T>& v)
	{
		*this *= Translation(v);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Translate(T x, T y, T z)
	{
		*this *= Translation(x, y, z);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::RotateX(T radians)
	{
		*this *= RotationX(radians);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::RotateY(T radians)
	{
		*this *= RotationY(radians);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::RotateZ(T radians)
	{
		*this *= RotationZ(radians);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Scale(const TVector3<T>& v)
	{
		*this *= Scaling(v);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Scale(T x, T y, T z)
	{
		*this *= Scaling(x, y, z);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Transpose()
	{
		Swap(m[0][1], m[1][0]);
		Swap(m[0][2], m[2][0]);
		Swap(m[0][3], m[3][0]);
		Swap(m[1][2], m[2][1]);
		Swap(m[1][3], m[3][1]);
		Swap(m[2][3], m[3][2]);
	}

	template<Numeric T>
	constexpr void TMatrix44<T>::Inverse()
	{
		const T det = Determinant();
		if (det == 0)
		{
			return;
		}

		const T invDet = 1 / det;

		const T a = m[0][0];
		const T b = m[0][1];
		const T c = m[0][2];
		const T d = m[0][3];
		const T e = m[1][0];
		const T f = m[1][1];
		const T g = m[1][2];
		const T h = m[1][3];
		const T i = m[2][0];
		const T j = m[2][1];
		const T k = m[2][2];
		const T l = m[2][3];
		const T m = m[3][0];
		const T n = m[3][1];
		const T o = m[3][2];
		const T p = m[3][3];

		m[0][0] = (f * k * p + g * l * n + h * j * o - f * l * o - g * j * p - h * k * n) * invDet;
		m[0][1] = (b * l * o + c * j * p + d * k * n - b * k * p - c * l * n - d * j * o) * invDet;
		m[0][2] = (b * g * p + c * h * n + d * f * o - b * h * o - c * f * p - d * g * n) * invDet;
		m[0][3] = (b * h * k + c * f * l + d * g * j - b * g * l - c * h * j - d * f * k) * invDet;
		m[1][0] = (e * l * o + g * i * p + h * k * m - e * k * p - g * l * m - h * i * o) * invDet;
		m[1][1] = (a * k * p + c * l * m + d * i * o - a * l * o - c * i * p - d * k * m) * invDet;
		m[1][2] = (a * h * o + c * f * p + d * g * m - a * g * p - c * h * m - d * f * o) * invDet;
		m[1][3] = (a * g * l + c * h * m + d * f * k - a * h * k - c * f * l - d * g * m) * invDet;
		m[2][0] = (e * j * p + f * l * m + h * i * n - e * l * n - f * i * p - h * j * m) * invDet;
		m[2][1] = (a * l * n + b * i * p + d * j * m - a * j * p - b * l * m - d * i * n) * invDet;
		m[2][2] = (a * f * p + b * h * m + d * e * n - a * h * n - b * e * p - d * f * m) * invDet;
		m[2][3] = (a * h * j + b * e * l + d * f * i - a * f * l - b * h * i - d * e * j) * invDet;
		m[3][0] = (e * k * n + f * i * o + g * j * m - e * j * o - f * k * m - g * i * n) * invDet;
		m[3][1] = (a * j * o + b * k * m + c * i * n - a * k * n - b * i * o - c * j * m) * invDet;
		m[3][2] = (a * g * n + b * e * o + c * f * m - a * f * o - b * g * m - c * e * n) * invDet;
		m[3][3] = (a * f * k + b * g * m + c * e * j - a * g * j - b * e * k - c * f * m) * invDet;
	}

	template<Numeric T>
	constexpr bool TMatrix44<T>::IsOrthogonal() const
	{
		const T dotX = m[0].Dot(m[1]);
		const T dotY = m[1].Dot(m[2]);
		const T dotZ = m[2].Dot(m[0]);

		return AreValuesClose(dotX, static_cast<T>(0)) &&
			   AreValuesClose(dotY, static_cast<T>(0)) &&
			   AreValuesClose(dotZ, static_cast<T>(0));
	}

	template<Numeric T>
	constexpr String TMatrix44<T>::ToString() const
	{
		return String::Format<256>("\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f", m[0][0], m[0][1], m[0][2], m[0][3],
			                                                                                                                           m[1][0], m[1][1], m[1][2], m[1][3],
			                                                                                                                           m[2][0], m[2][1], m[2][2], m[2][3],
			                                                                                                                           m[3][0], m[3][1], m[3][2], m[3][3]);
	}

	template<Numeric T>
	constexpr bool TMatrix44<T>::operator==(const TMatrix44<T>& rhs) const
	{
		return m[0] == rhs.m[0] &&
			   m[1] == rhs.m[1] &&
			   m[2] == rhs.m[2] &&
			   m[3] == rhs.m[3];
	}
}

export using Matrix44  = jpt::TMatrix44<Precision>;
export using Matrix44f = jpt::TMatrix44<float32>;