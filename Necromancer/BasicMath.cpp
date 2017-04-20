#include "stdafx.h"
#include "BasicMath.h"

#include "Algorithm.hpp"
#include "Exception.h"

namespace Necromancer{


	Vec2::Vec2(float x, float y)
		:x(x), y(y)
	{
	}

	float& Vec2::operator[](unsigned long i) {
#ifdef _DEBUG
		if (i >= 2)
			throw std::exception("vec2:index out of range");
		else
			return data[i];
#else
		return data[i];
#endif
	}

	const float& Vec2::operator[](unsigned long i) const {
#ifdef _DEBUG
		if (i >= 2)
			throw std::exception("vec2:index out of range");
		else
			return data[i];
#else
		return data[i];
#endif
	}

	const Vec2 Vec2::operator+(const Vec2& v) const {
		return Vec2(x + v.x, y + v.y);
	}

	const Vec2 Vec2::operator-(const Vec2& v) const {
		return Vec2(x - v.x, y - v.y);
	}

	const Vec2 Vec2::operator*(const Vec2& v) const {
		return Vec2(x * v.x, y * v.y);
	}

	const Vec2 Vec2::operator/(const Vec2& v) const {
		return Vec2(x / v.x, y / v.y);
	}

	const Vec2 Vec2::operator*(const float val) const {
		return Vec2(x * val, y * val);
	}

	const Vec2 Vec2::operator/(const float val) const {
		return Vec2(x / val, y / val);
	}

	const Vec2 operator*(const float val, const Vec2& vec) {
		return Vec2(vec.x * val, vec.y * val);
	}

	const float dot(const Vec2& v1, const Vec2& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	const float length(const Vec2& vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}

	Vec3::Vec3(float x, float y, float z)
		:x(x), y(y), z(z)
	{
	}

	Vec3::Vec3(const Vec2& xy, float z)
		: xy(xy), z(z)
	{
	}

	Vec3::Vec3(float x, const Vec2& yz)
		: x(x), yz(yz)
	{
	}

	float& Vec3::operator[](unsigned long i) {
#ifdef _DEBUG
		if (i >= 3)
			throw std::exception("Vec3:index out of range");
		else
			return data[i];
#else
		return data[i];
#endif
	}

	const float& Vec3::operator[](unsigned long i)const {
#ifdef _DEBUG
		if (i >= 3)
			throw std::exception("Vec3:index out of range");
		else
			return data[i];
#else
		return data[i];
#endif
	}

	const Vec3 Vec3::operator+(const Vec3& v)const {
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	const Vec3 Vec3::operator-(const Vec3& v)const {
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	const Vec3 Vec3::operator*(const Vec3& v)const {
		return Vec3(x * v.x, y * v.y, z * v.z);
	}

	const Vec3 Vec3::operator/(const Vec3& v)const {
		return Vec3(x / v.x, y / v.y, z / v.z);
	}

	const Vec3 Vec3::operator*(const float val)const {
		return Vec3(x * val, y * val, z * val);
	}

	const Vec3 Vec3::operator/(const float val)const {
		return Vec3(x / val, y / val, z / val);
	}

	const Vec3 operator*(const float val, const Vec3& vec) {
		return Vec3(vec.x * val, vec.y * val, vec.z * val);
	}

	const float dot(const Vec3& v1, const Vec3& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	const Vec3 cross(const Vec3& v1, const Vec3& v2) {
		return Vec3(v1[1] * v2[2] - v2[1] * v1[2],
			v1[2] * v2[0] - v2[2] * v1[0],
			v1[0] * v2[1] - v2[0] * v1[1]);
	}

	const float length(const Vec3& vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}

	const Vec3 normalize(const Vec3& vec) {
		return vec / length(vec);
	}


	Vec4::Vec4(float x, float y, float z, float w)
		:x(x), y(y), z(z), w(w)
	{
	}

	Vec4::Vec4(const Vec3& xyz, float w)
		: xyz(xyz), w(w)
	{
	}

	Vec4::Vec4(float x, const Vec3& yzw)
		: x(x), yzw(yzw)
	{
	}

	Vec4::Vec4(const Vec2& xy, const Vec2& zw)
		: xy(xy), zw(zw)
	{
	}

	float& Vec4::operator[](unsigned long i) {
#ifdef _DEBUG
		if (i >= 4)
			throw std::exception("Vec4:index out of range");
		else
			return data[i];
#else
		return data[i];
#endif
	}

	const float& Vec4::operator[](unsigned long i) const {
#ifdef _DEBUG
		if (i >= 4)
			throw std::exception("Vec4:index out of range");
		else
			return data[i];
#else
		return data[i];
#endif
	}

	const Vec4 Vec4::operator+(const Vec4& v)const {
		_declspec(align(16)) Vec4 v1(*this);
		_declspec(align(16)) Vec4 v2(v);
		__m128 a = _mm_load_ps((const float*)&v1);
		__m128 b = _mm_load_ps((const float*)&v2);
		__m128 c = _mm_add_ps(a, b);
		_declspec(align(16)) Vec4 result;
		_mm_store_ps((float*)&result, c);
		return result;
	}

	const Vec4 Vec4::operator-(const Vec4& v)const {
		_declspec(align(16)) Vec4 v1(*this);
		_declspec(align(16)) Vec4 v2(v);
		__m128 a = _mm_load_ps((const float*)&v1);
		__m128 b = _mm_load_ps((const float*)&v2);
		__m128 c = _mm_sub_ps(a, b);
		_declspec(align(16)) Vec4 result;
		_mm_store_ps((float*)&result, c);
		return result;
	}

	const Vec4 Vec4::operator*(const Vec4& v)const {
		_declspec(align(16)) Vec4 v1(*this);
		_declspec(align(16)) Vec4 v2(v);
		__m128 a = _mm_load_ps((const float*)&v1);
		__m128 b = _mm_load_ps((const float*)&v2);
		__m128 c = _mm_mul_ps(a, b);
		_declspec(align(16)) Vec4 result;
		_mm_store_ps((float*)&result, c);
		return result;
	}

	const Vec4 Vec4::operator/(const Vec4& v)const {
		_declspec(align(16)) Vec4 v1(*this);
		_declspec(align(16)) Vec4 v2(v);
		__m128 a = _mm_load_ps((const float*)&v1);
		__m128 b = _mm_load_ps((const float*)&v2);
		__m128 c = _mm_div_ps(a, b);
		_declspec(align(16)) Vec4 result;
		_mm_store_ps((float*)&result, c);
		return result;
	}

	const Vec4 Vec4::operator*(const float val)const {
		return (*this) * Vec4(val, val, val, val);
	}

	const Vec4 Vec4::operator/(const float val)const {
		return Vec4(x / val, y / val, z / val, w / val);
	}

	const Vec4 operator*(const float val, const Vec4& vec) {
		return vec * val;
	}

	const float dot(const Vec4& v1, const Vec4& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	const float length(const Vec4& v) {
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	}

	const Vec4 normalize(const Vec4& v) {
		return v / length(v);
	}



	Mat3::Mat3() {
	}

	Mat3::Mat3(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22)
		:m0(m00, m01, m02),
		m1(m10, m11, m12),
		m2(m20, m21, m22)
	{
	}

	Mat3::Mat3(const Vec3& m0,
		const Vec3& m1,
		const Vec3& m2)
		:m0(m0), m1(m1), m2(m2)
	{
	}

	Vec3& Mat3::operator[](unsigned long i) {
		switch (i) {
		case 0:
			return m0;
		case 1:
			return m1;
		case 2:
			return m2;
		default:
#ifdef _DEBUG
			throw std::exception("Mat3: index out of range");
#else
			break;
#endif
		}
	}

	const Vec3& Mat3::operator[](unsigned long i)const {
		switch (i) {
		case 0:
			return m0;
		case 1:
			return m1;
		case 2:
			return m2;
		default:
#ifdef _DEBUG
			throw std::exception("Mat3: index out of range");
#else
			break;
#endif
		}
	}

	const Mat3 Mat3::operator+(const Mat3& m) const {
		Mat3 result;
		for (unsigned long i = 0; i < 3; ++i) {
			for (unsigned long j = 0; j < 3; ++j) {
				result[i][j] = (*this)[i][j] + m[i][j];
			}
		}
		return result;
	}

	const Mat3 Mat3::operator-(const Mat3& m)const {
		Mat3 result;
		for (unsigned long i = 0; i < 3; ++i) {
			for (unsigned long j = 0; j < 3; ++j) {
				result[i][j] = (*this)[i][j] - m[i][j];
			}
		}
		return result;
	}

	const Mat3 Mat3::operator*(const Mat3& m)const {
		Mat3 result;
		for (unsigned long i = 0; i < 3; ++i) {
			for (unsigned long j = 0; j < 3; ++j) {
				result[i][j] = 0.0f;
				for (unsigned long k = 0; k < 3; ++k) {
					result[i][j] += (*this)[i][k] * m[k][j];
				}
			}
		}
		return result;
	}

	const Mat3 Mat3::operator*(const float val)const {
		Mat3 result;
		for (unsigned long i = 0; i < 3; ++i) {
			for (unsigned long j = 0; j < 3; ++j) {
				result[i][j] = (*this)[i][j] * val;
			}
		}
		return result;
	}

	const Mat3 Mat3::operator/(const float val)const {
		Mat3 result;
		for (unsigned long i = 0; i < 3; ++i) {
			for (unsigned long j = 0; j < 3; ++j) {
				result[i][j] = (*this)[i][j] / val;
			}
		}
		return result;
	}

	const Vec3 operator*(const Vec3& vec, const Mat3& mat) {
		Vec3 result;
		for (unsigned long i = 0; i < 3; ++i) {
			result[i] = 0.0f;
			for (unsigned long j = 0; j < 3; ++j) {
				result[i] += vec[j] * mat[j][i];
			}
		}
		return result;
	}




	_Mat4::_Mat4() {
	}

	_Mat4::_Mat4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
		:m0(m00, m01, m02, m03),
		m1(m10, m11, m12, m13),
		m2(m20, m21, m22, m23),
		m3(m30, m31, m32, m33)
	{
	}

	_Mat4::_Mat4(const Vec4& m0,
		const Vec4& m1,
		const Vec4& m2,
		const Vec4& m3)
		:m0(m0), m1(m1), m2(m2), m3(m3)
	{
	}

	Vec4& _Mat4::operator[](unsigned long i) {
#ifdef _DEBUG
		if ((i > 3) || (i < 0))
			throw std::exception("Mat4:index out of range");
#endif
		auto vec_array = (Vec4*)this;
		return vec_array[i];
	}

	const Vec4& _Mat4::operator[](unsigned long i) const {
#ifdef _DEBUG
		if ((i > 3) || (i < 0))
			throw std::exception("Mat4:index out of range");
#endif
		auto vec_array = (Vec4*)this;
		return vec_array[i];
	}

	const Mat4 _Mat4::operator+(const Mat4& m)const {
		return Mat4(
			m0 + m.m0,
			m1 + m.m1,
			m2 + m.m2,
			m3 + m.m3);
	}

	const Mat4 _Mat4::operator-(const Mat4& m)const {
		return Mat4(
			m0 - m.m0,
			m1 - m.m1,
			m2 - m.m2,
			m3 - m.m3);
	}

	const Mat4 _Mat4::operator*(const Mat4& p_m)const {
		//Mat4 result;
		__declspec(align(16)) float f_r[16];
		Mat4& result = *((Mat4*)f_r);
		__m128 row[4];
		for (unsigned long i = 0; i < 4; ++i) {
			row[i] = _mm_load_ps((const float*)&p_m[i]);
		}
		for (unsigned long i = 0; i < 4; ++i) {
			__m128 v = _mm_load_ps((const float*)&((*this)[i]));
			__m128 c = _mm_mul_ps(row[0],
				_mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)));
			__m128 d = _mm_mul_ps(row[1],
				_mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
			c = _mm_add_ps(c, d);
			d = _mm_mul_ps(row[2],
				_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
			c = _mm_add_ps(c, d);
			d = _mm_mul_ps(row[3],
				_mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));
			c = _mm_add_ps(c, d);
			_mm_store_ps((float*)&result[i], c);
		}
		return result;
	}

	const Mat4 _Mat4::operator*(const float val)const {
		Mat4 result;
		for (unsigned long i = 0; i < 4; ++i) {
			for (unsigned long j = 0; j < 4; ++j) {
				result[i][j] = (*this)[i][j] * val;
			}
		}
		return result;
	}

	const Mat4 _Mat4::operator/(const float val)const {
		Mat4 result;
		for (unsigned long i = 0; i < 4; ++i) {
			for (unsigned long j = 0; j < 4; ++j) {
				result[i][j] = (*this)[i][j] / val;
			}
		}
		return result;
	}

	const Vec4 operator*(const Vec4& p_vec, const Mat4& p_mat) {
		_declspec(align(16)) Mat4 mat(p_mat);
		_declspec(align(16)) Vec4 vec(p_vec);
		__m128 row[4];
		for (unsigned long i = 0; i < 4; ++i) {
			row[i] = _mm_load_ps((const float*)&mat[i]);
		}
		__m128 v = _mm_load_ps((const float*)&vec);
		row[0] = _mm_mul_ps(row[0],
			_mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)));
		row[1] = _mm_mul_ps(row[1],
			_mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
		row[2] = _mm_mul_ps(row[2],
			_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
		row[3] = _mm_mul_ps(row[3],
			_mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));
		__m128 r = row[0];
		for (unsigned long i = 1; i < 4; ++i) {
			r = _mm_add_ps(r, row[i]);
		}
		_declspec(align(16)) Vec4 result;
		_mm_store_ps((float*)&result, r);
		return result;
	}


	Quaternion::Quaternion() {
		m_data = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Quaternion::Quaternion(const Vec4& v)
		:m_data(v)
	{
	}

	Quaternion::Quaternion(float x, float y, float z, float w)
		: m_data(x, y, z, w)
	{
	}

	Quaternion::~Quaternion() {
	}

	const Quaternion Quaternion::operator*(const Quaternion& q) const {
		Vec3 data_xyz, q_data_xyz;
		q_data_xyz[0] = q.m_data[0];
		q_data_xyz[1] = q.m_data[1];
		q_data_xyz[2] = q.m_data[2];
		data_xyz[0] = m_data[0];
		data_xyz[1] = m_data[1];
		data_xyz[2] = m_data[2];
		Vec3 xyz = m_data[3] * q_data_xyz +
			data_xyz * q.m_data[3] +
			cross(data_xyz, q_data_xyz);
		float w = m_data[3] * q.m_data[3] -
			dot(data_xyz, q_data_xyz);
		return Quaternion(Vec4(xyz, w));
	}

	Vec4 Quaternion::rotate_vec4(const Vec4& v) {
		Vec3 tmp;
		tmp[0] = v[0];
		tmp[1] = v[1];
		tmp[2] = v[2];
		const Vec3& vec_ref = tmp;
		return Vec4(rotate_vec3(vec_ref), v[3]);
	}

	Vec3 Quaternion::rotate_vec3(const Vec3& v) {
		Quaternion inverse_quat = inverse_quaternion(*this);
		Vec4 v4(v, 0.0f);
		Quaternion q_v4(v4);
		Quaternion result =
			(*this) * q_v4 * inverse_quat;
		Vec3 rs_v3;
		rs_v3[0] = result[0];
		rs_v3[1] = result[1];
		rs_v3[2] = result[2];
		return rs_v3;
	}

	Mat3 Quaternion::convert_to_matrix3x3() const {
		float x = m_data[0];
		float y = m_data[1];
		float z = m_data[2];
		float w = m_data[3];
		Mat3 m;
		float length = this->length();
		float length2 = length * length;
		if (fabs(length2) <= std::numeric_limits<double>::min()) {
			m[0][0] = 0.0f; m[1][0] = 0.0f; m[2][0] = 0.0f;
			m[0][1] = 0.0f; m[1][1] = 0.0f; m[2][1] = 0.0f;
			m[0][2] = 0.0f; m[1][2] = 0.0f; m[2][2] = 0.0f;
		}
		else {
			float rlength2;
			if (length2 != 1.0f)
			{
				rlength2 = 2.0f / length2;
			}
			else
			{
				rlength2 = 2.0f;
			}

			float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

			x2 = rlength2*x;
			y2 = rlength2*y;
			z2 = rlength2*z;

			xx = x * x2;
			xy = x * y2;
			xz = x * z2;

			yy = y * y2;
			yz = y * z2;
			zz = z * z2;

			wx = w * x2;
			wy = w * y2;
			wz = w * z2;

			m[0][0] = 1.0f - (yy + zz);
			m[1][0] = xy - wz;
			m[2][0] = xz + wy;


			m[0][1] = xy + wz;
			m[1][1] = 1.0f - (xx + zz);
			m[2][1] = yz - wx;

			m[0][2] = xz - wy;
			m[1][2] = yz + wx;
			m[2][2] = 1.0f - (xx + yy);
		}

		return m;
	}

	Mat4 Quaternion::convert_to_matrix4x4() const {
		float x = m_data[0];
		float y = m_data[1];
		float z = m_data[2];
		float w = m_data[3];
		/*return Mat4(1 - 2*y*y - 2*z*z,	2*x*y + 2*z*w,		2*x*z - 2*y*w,		0.0f,
		2*x*y - 2*z*w,		1 - 2*x*x - 2*z*z,	2*y*z + 2*x*w,		0.0f,
		2*x*z + 2*y*w,		2*y*w - 2*x*w,		1 - 2*x*x - 2*y*y,	0.0f,
		0.0f,				0.0f,				0.0f,				1.0f);*/

		Mat4 m;
		float length = this->length();
		float length2 = length * length;
		if (fabs(length2) <= std::numeric_limits<double>::min()) {
			m[0][0] = 0.0f; m[1][0] = 0.0f; m[2][0] = 0.0f; m[3][0] = 0.0f;
			m[0][1] = 0.0f; m[1][1] = 0.0f; m[2][1] = 0.0f; m[3][1] = 0.0f;
			m[0][2] = 0.0f; m[1][2] = 0.0f; m[2][2] = 0.0f; m[3][2] = 0.0f;
			m[0][3] = 0.0f; m[1][3] = 0.0f; m[2][3] = 0.0f; m[3][3] = 1.0f;
		}
		else {
			float rlength2;
			if (length2 != 1.0f)
			{
				rlength2 = 2.0f / length2;
			}
			else
			{
				rlength2 = 2.0f;
			}

			float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

			x2 = rlength2*x;
			y2 = rlength2*y;
			z2 = rlength2*z;

			xx = x * x2;
			xy = x * y2;
			xz = x * z2;

			yy = y * y2;
			yz = y * z2;
			zz = z * z2;

			wx = w * x2;
			wy = w * y2;
			wz = w * z2;

			m[0][0] = 1.0f - (yy + zz);
			m[1][0] = xy - wz;
			m[2][0] = xz + wy;
			m[3][0] = 0.0f;


			m[0][1] = xy + wz;
			m[1][1] = 1.0f - (xx + zz);
			m[2][1] = yz - wx;
			m[3][1] = 0.0f;

			m[0][2] = xz - wy;
			m[1][2] = yz + wx;
			m[2][2] = 1.0f - (xx + yy);
			m[3][2] = 0.0f;

			m[0][3] = 0.0f;
			m[1][3] = 0.0f;
			m[2][3] = 0.0f;
			m[3][3] = 1.0f;
		}

		return m;
	}

	float Quaternion::length() const {
		return sqrt(
			m_data[0] * m_data[0] +
			m_data[1] * m_data[1] +
			m_data[2] * m_data[2] +
			m_data[3] * m_data[3]);
	}

	float& Quaternion::operator[](unsigned long i) {
		return m_data[i];
	}

	const float& Quaternion::operator[](unsigned long i) const {
		return m_data[i];
	}

	Quaternion mat3_to_quaternion(const Mat3& R) {
		Quaternion q;

		float trace = R[0][0] + R[1][1] + R[2][2];
		if (trace > 0.0f) {
			float s = sqrt(trace + 1.0f);
			q[3] = s * 0.5f;

			float t = 0.5f / s;
			q[0] = (R[2][1] - R[1][2]) * t;
			q[1] = (R[0][2] - R[2][0]) * t;
			q[2] = (R[1][0] - R[0][1]) * t;
		}
		else {
			unsigned long i = 0;
			if (R[1][1] > R[0][0]) i = 1;
			if (R[2][2] > R[i][i]) i = 2;

			static const unsigned long NEXT[3] = { 1, 2, 0 };
			unsigned long j = NEXT[i];
			unsigned long k = NEXT[j];

			float s = sqrt((R[i][i] -
				(R[j][j] + R[k][k])) + 1.0f);
			q[i] = s * 0.5f;

			float t;
			if (s != 0.0f) t = 0.5f / s;
			else t = s;

			q[3] = (R[k][j] - R[j][k]) * t;
			q[j] = (R[j][i] + R[i][j]) * t;
			q[k] = (R[k][i] + R[i][k]) * t;
		}

		return q;
	}

	Quaternion mat4_to_quaternion(const Mat4& R) {
		Mat3 tmp;
		for (unsigned long i = 0; i < 3; ++i) {
			for (unsigned long j = 0; j < 3; ++j) {
				tmp[i][j] = R[i][j];
			}
		}
		return mat3_to_quaternion(tmp);
	}

	Quaternion inverse_quaternion(const Quaternion& q) {
		return Quaternion(-q[0], -q[1], -q[2], q[3]);
	}

	Quaternion blend_lerp(float b,
		const Quaternion& q1, const Quaternion& q2)
	{
		Vec4 v1(q1[0], q1[1], q1[2], q1[3]);
		Vec4 v2(q2[0], q2[1], q2[2], q2[3]);
		v1 = (1.0f - b) * v1;
		v2 = b * v2;
		v1 = v1 + v2;
		v1 = v1 / length(v1);
		return Quaternion(v1);
	}

	Quaternion blend_slerp(float b,
		const Quaternion& q1, const Quaternion& q2)
	{
		Vec4 v1(q1[0], q1[1], q1[2], q1[3]);
		Vec4 v2(q2[0], q2[1], q2[2], q2[3]);
		float cos_theta = dot(v1.xyz, v2.xyz);
		float theta = acosf(cos_theta);
		float wp = sin((1 - b) * theta) / sin(theta);
		float wq = sin(b * theta) / sin(theta);
		return Quaternion(wp * v1 + wq * v2);
	}

	Quaternion make_quanternion(const Vec3& axis, float theta) {
		theta = theta / 180.0f * 3.1415926f;
		return Quaternion(sin(theta / 2.0f) * axis.x,
			sin(theta / 2.0f) * axis.y,
			sin(theta / 2.0f) * axis.z,
			cos(theta / 2.0f));
	}

	Vec3 Quaternion::get_axis() {
		float theta_div_2 = acosf(m_data[3]);
		Vec3 axis = m_data.xyz / sin(theta_div_2);
		return axis;
	}

	float Quaternion::get_theta() {
		float theta = acosf(m_data[3]) * 2.0f;
		theta = theta / 3.1415926f * 180.0f;
		return theta;
	}

	Quaternion make_quanternion_from_euler(const Vec3& euler_angle) {
		Vec4 v;
		Vec3 t = euler_angle / 2.0f;
		v.w = cos(t.x) * cos(t.y) * cos(t.z) +
			sin(t.x) * sin(t.y) * sin(t.z);
		v.x = cos(t.x) * cos(t.y) * sin(t.z) -
			sin(t.x) * sin(t.y) * cos(t.z);
		v.y = cos(t.x) * sin(t.y) * cos(t.z) +
			sin(t.x) * cos(t.y) * sin(t.z);
		v.z = sin(t.x) * cos(t.y) * cos(t.z) -
			cos(t.x) * sin(t.y) * sin(t.z);
		return Quaternion(v);
	}


	Mat4 translate_matrix(float x, float y, float z) {
		Mat4 result;
		memset(&result, 0, 16 * sizeof(float));
		result[0][0] = 1.0f;
		result[1][1] = 1.0f;
		result[2][2] = 1.0f;
		result[3][3] = 1.0f;
		result[3][0] = x;
		result[3][1] = y;
		result[3][2] = z;
		return result;
	}

	Mat4 translate_matrix(const Vec3& v) {
		return translate_matrix(v[0], v[1], v[2]);
	}

	Mat4 rotate_matrix(float ax, float ay, float az, float theta) {
		const float pi = 3.1415926f;
		const float half = 180.0f;

		Vec3 rotate_axis(ax, ay, az);
		rotate_axis = normalize(rotate_axis);
		float radian = theta / half * pi;
		float cosA = cosf(radian);
		float oneC = 1 - cosA;
		float sinA = sinf(radian);
		float ux = rotate_axis[0];
		float uy = rotate_axis[1];
		float uz = rotate_axis[2];

		Mat4 result;
		memset(&result, 0, sizeof(float) * 16);
		result[0][0] = ux*ux*oneC + cosA;
		result[0][1] = uy*ux*oneC + uz*sinA;
		result[0][2] = uz*ux*oneC - uy*sinA;

		result[1][0] = ux*uy*oneC - uz*sinA;
		result[1][1] = uy*uy*oneC + cosA;
		result[1][2] = uz*uy*oneC + ux*sinA;

		result[2][0] = ux*uz*oneC + uy*sinA;
		result[2][1] = uy*uz*oneC - ux*sinA;
		result[2][2] = uz*uz*oneC + cosA;

		result[3][3] = 1.0f;

		return result;
	}

	Mat4 rotate_matrix(const Vec3& axis, float theta) {
		return rotate_matrix(axis[0], axis[1], axis[2], theta);
	}

	Mat4 rotate_matrix_vec_to_vec(const Vec3& from, const Vec3& to) {
		const float pi = 3.141592653f;
		const float half = 180.0f;
		Vec3 normal_vector = cross(from, to);
		float cos_theta = dot(normalize(from), normalize(to));
		if (cos_theta >= 0.99f) {
			Mat4 result(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
			return result;
		}
		float theta = acos(cos_theta);
		theta = theta / pi * half;

		return rotate_matrix(normal_vector, theta);
	}

	Mat4 scale_matrix(float sx, float sy, float sz) {
		Mat4 result;
		memset(&result, 0, 16 * sizeof(float));
		result[0][0] = sx;
		result[1][1] = sy;
		result[2][2] = sz;
		result[3][3] = 1.0f;
		return result;
	}

	Mat4 scale_matrix(const Vec3& v) {
		return scale_matrix(v[0], v[1], v[2]);
	}

	Mat4 perspective_projection_matrix(float aspect, float theta,
		float znear, float zfar)
	{
		const float pi = 3.14159f;
		const float half = 180.0f;
		float radian = theta / half * pi;
		float cot = 1.0f / tan(radian / 2.0f);
		Mat4 result;
		memset(&result, 0, 16 * sizeof(float));
		result[0][0] = cot / aspect;
		result[1][1] = cot;
		result[2][2] = (znear + zfar) / (zfar - znear);
		result[2][3] = 2 * znear * zfar / (zfar - znear);
		result[3][2] = -1.0f;
		return result;
	}

	Mat4 orthographic_projection_matrix(float xmin, float xmax,
		float ymin, float ymax, float znear, float zfar)
	{
		Mat4 result;
		memset(&result, 0, 16 * sizeof(float));

		result[0][0] = 2.0f / (xmax - xmin);
		result[1][1] = 2.0f / (ymax - ymin);
		result[2][2] = -1.0f / (znear - zfar);
		result[3][0] = (xmax + xmin) / (xmax - xmin);
		result[3][1] = (ymax + ymin) / (ymax - ymin);
		result[3][2] = znear / (znear - zfar);
		result[3][3] = 1.0f;

		return result;
	}

	Mat4 inverse_matrix(const Mat4& matrix) {
		Mat4 m(matrix);
		unsigned long js[4];
		unsigned long is[4];
		float f_det = 1.0f;
		int f = 1;
		for (unsigned long k = 0; k < 4; ++k) {
			float f_max = 0.0f;
			for (unsigned long i = k; i < 4; ++i) {
				for (unsigned long j = k; j < 4; ++j) {
					float f = abs(m[i][j]);
					if (f > f_max) {
						f_max = f;
						is[k] = i;
						js[k] = j;
					}
				}
			}
			if (abs(f_max) < 0.0001f)
				return Mat4();
			if (is[k] != k) {
				f = -f;
				std::swap(m[k][0], m[is[k]][0]);
				std::swap(m[k][1], m[is[k]][1]);
				std::swap(m[k][2], m[is[k]][2]);
				std::swap(m[k][3], m[is[k]][3]);
			}
			if (js[k] != k) {
				f = -f;
				std::swap(m[0][k], m[0][js[k]]);
				std::swap(m[1][k], m[1][js[k]]);
				std::swap(m[2][k], m[2][js[k]]);
				std::swap(m[3][k], m[3][js[k]]);
			}

			f_det *= m[k][k];
			m[k][k] = 1.0f / m[k][k];
			for (unsigned long j = 0; j < 4; ++j) {
				if (j != k)
					m[k][j] *= m[k][k];
			}
			for (unsigned long i = 0; i < 4; ++i) {
				if (i != k) {
					for (unsigned long j = 0; j < 4; ++j) {
						if (j != k)
							m[i][j] = m[i][j] - m[i][k] * m[k][j];
					}
				}
			}
			for (unsigned long i = 0; i < 4; ++i) {
				if (i != k)
					m[i][k] *= -m[k][k];
			}
		}

		for (int k = 3; k >= 0; --k) {
			if (js[k] != k) {
				std::swap(m[k][0], m[js[k]][0]);
				std::swap(m[k][1], m[js[k]][1]);
				std::swap(m[k][2], m[js[k]][2]);
				std::swap(m[k][3], m[js[k]][3]);
			}
			if (is[k] != k) {
				std::swap(m[0][k], m[0][is[k]]);
				std::swap(m[1][k], m[1][is[k]]);
				std::swap(m[2][k], m[2][is[k]]);
				std::swap(m[3][k], m[3][is[k]]);
			}
		}

		return m;
	}

	Mat4 look_at_matrix(const Vec3& eye,
		const Vec3& target, const Vec3& up)
	{
		Vec3 z_axis = normalize(target - eye);
		Vec3 x_axis = normalize(cross(up, z_axis));
		Vec3 y_axis = cross(z_axis, x_axis);

		Mat4 orientation(x_axis.x, y_axis.x, z_axis.x, 0,
			x_axis.y, y_axis.y, z_axis.y, 0,
			x_axis.z, y_axis.z, z_axis.z, 0,
			0, 0, 0, 1);

		Mat4 translation(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-eye.x, -eye.y, -eye.z, 1);

		return (translation * orientation);
	}

	Mat4 look_at_matrix_by_look_at_dir(
		const Vec3& eye, const Vec3& look_at, const Vec3& up)
	{
		Vec3 z_axis = normalize(look_at);
		Vec3 x_axis = normalize(cross(up, z_axis));
		Vec3 y_axis = cross(z_axis, x_axis);

		Mat4 orientation(x_axis.x, y_axis.x, z_axis.x, 0,
			x_axis.y, y_axis.y, z_axis.y, 0,
			x_axis.z, y_axis.z, z_axis.z, 0,
			0, 0, 0, 1);

		Mat4 translation(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-eye.x, -eye.y, -eye.z, 1);

		return (translation * orientation);
	}
}