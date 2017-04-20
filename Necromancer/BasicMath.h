#pragma once

#include "CommonUtility.h"

namespace Necromancer {

	//2D vector
	union Vec2 {
		struct {
			float x, y;
		};

		struct {
			float data[2];
		};

		Vec2() {}
		Vec2(float, float);

		float& operator[](unsigned long i);
		const float& operator[](unsigned long i) const;

		const Vec2 operator+(const Vec2&) const;
		const Vec2 operator-(const Vec2&) const;
		const Vec2 operator*(const Vec2&) const;
		const Vec2 operator/(const Vec2&) const;
		const Vec2 operator*(const float) const;
		const Vec2 operator/(const float) const;
	};

	const Vec2 operator*(const float, const Vec2&);

	//dot product
	const float dot(const Vec2&, const Vec2&);

	//length
	const float length(const Vec2&);

	//3D Vector
	union  Vec3 {
		struct {
			float x, y, z;
		};
		struct {
			float data[3];
		};
		struct {
			Vec2 xy;
			float z;
		};
		struct {
			float x;
			Vec2 yz;
		};

		Vec3() {}
		Vec3(float, float, float);
		Vec3(const Vec2&, float);
		Vec3(float, const Vec2&);

		float& operator[](unsigned long i);
		const float& operator[](unsigned long i)const;

		const Vec3 operator+(const Vec3&) const;
		const Vec3 operator-(const Vec3&) const;
		const Vec3 operator*(const Vec3&) const;
		const Vec3 operator/(const Vec3&) const;
		const Vec3 operator*(const float) const;
		const Vec3 operator/(const float) const;
	};

	const Vec3 operator*(const float, const Vec3&);

	//dot product
	const float dot(const Vec3&, const Vec3&);

	//cross product
	const Vec3 cross(const Vec3&, const Vec3&);

	//length
	const float length(const Vec3&);

	//normalize
	const Vec3 normalize(const Vec3&);

	//4D vector
	union Vec4 {
		struct {
			float x, y, z, w;
		};
		struct {
			float data[4];
		};
		struct {
			Vec3 xyz;
			float w;
		};
		struct {
			float x;
			Vec3 yzw;
		};
		struct {
			Vec2 xy;
			Vec2 zw;
		};

		Vec4() {}
		Vec4(float, float, float, float);
		Vec4(const Vec3&, float);
		Vec4(float, const Vec3&);
		Vec4(const Vec2&, const Vec2&);

		float& operator[](unsigned long i);
		const float& operator[](unsigned long i)const;

		const Vec4 operator+(const Vec4&) const;
		const Vec4 operator-(const Vec4&) const;
		const Vec4 operator*(const Vec4&) const;
		const Vec4 operator/(const Vec4&) const;
		const Vec4 operator*(const float) const;
		const Vec4 operator/(const float) const;
	};

	const Vec4 operator*(const float, const Vec4&);

	//dot product
	const float dot(const Vec4&, const Vec4&);

	//length
	const float length(const Vec4&);

	//normalize
	const Vec4 normalize(const Vec4&);


	//3D Matrix
	class Mat3 {
	public:
		Mat3();
		Mat3(float, float, float,
			float, float, float,
			float, float, float);
		Mat3(const Vec3&,
			const Vec3&,
			const Vec3&);

		Vec3& operator[](unsigned long i);
		const Vec3& operator[](unsigned long i)const;

		const Mat3 operator+(const Mat3&) const;
		const Mat3 operator-(const Mat3&) const;
		const Mat3 operator*(const Mat3&) const;
		const Mat3 operator*(const float) const;
		const Mat3 operator/(const float) const;
	private:
		Vec3 m0, m1, m2;
	};

	const Vec3 operator*(const Vec3&, const Mat3&);




	class _Mat4;

	//4D matrix
	//make it memory aligned so it would
	//support SSE
	typedef __declspec(align(16)) _Mat4 Mat4;

	class _Mat4 {
	public:
		_Mat4(float, float, float, float,
			float, float, float, float,
			float, float, float, float,
			float, float, float, float);
		_Mat4(const Vec4&,
			const Vec4&,
			const Vec4&,
			const Vec4&);
		_Mat4();

		Vec4& operator[](unsigned long i);
		const Vec4& operator[](unsigned long i)const;

		const Mat4 operator+(const Mat4&)const;
		const Mat4 operator-(const Mat4&)const;
		const Mat4 operator*(const Mat4&)const;
		const Mat4 operator*(const float)const;
		const Mat4 operator/(const float)const;
	private:
		Vec4 m0, m1, m2, m3;
	};

	const Vec4 operator*(const Vec4&, const Mat4&);



	//Quaternion
	class Quaternion {
	public:
		Quaternion();
		Quaternion(const Vec4&);
		Quaternion(float, float, float, float);
		~Quaternion();

		const Quaternion operator*(const Quaternion&) const;
		Vec4 rotate_vec4(const Vec4&);
		Vec3 rotate_vec3(const Vec3&);
		Mat3 convert_to_matrix3x3() const;
		Mat4 convert_to_matrix4x4() const;
		float& operator[](unsigned long i);
		float length() const;
		const float& operator[](unsigned long i) const;

		Vec3 get_axis();
		float get_theta();
	private:
		Vec4 m_data;
	private:
	};

	Quaternion inverse_quaternion(const Quaternion&);

	Quaternion mat3_to_quaternion(const Mat3&);

	Quaternion mat4_to_quaternion(const Mat4&);

	//create a quaternion to represent a rotation
	Quaternion make_quanternion(const Vec3& axis, float theta);

	//create a quaternion to represent a rotation as a certain euler angle
	Quaternion make_quanternion_from_euler(const Vec3&);

	//blend 2 quaternions using lerp way
	Quaternion blend_lerp(float b,
		const Quaternion& q1, const Quaternion& q2);

	//blend 2 quaternions using slerp way
	Quaternion blend_slerp(float b,
		const Quaternion& q1, const Quaternion& q2);



	//Special matrices

	//translate matrix
	//x --  offset in X
	//y --  offset in Y
	//z --  offset in Z
	Mat4 translate_matrix(float x, float y, float z);

	//translate matrix
	//vector of the offset
	Mat4 translate_matrix(const Vec3& xyz);

	//rotation matrix
	//axis_x  rotation axis in x
	//axis_y  rotation axis in y
	//axis_z  rotation axis in z
	//theta	  rotation degree
	//please make sure the rotation axis is nomralized
	Mat4 rotate_matrix(float axis_x, float axis_y, float axis_z, float theta);

	//rotation matrix
	//axis   rotation axis
	//theta  rotation degree
	//please make sure the rotation axis is nomralized
	Mat4 rotate_matrix(const Vec3& axis, float theta);

	//rotation matrix
	//from   from axis
	//to     to axis
	//please make sure the rotation axis is nomralized
	Mat4 rotate_matrix_vec_to_vec(const Vec3& from, const Vec3& to);

	//scale matrix
	//x		scale parm in x
	//y		scale parm in y
	//z		scale parm in z
	Mat4 scale_matrix(float x, float y, float z);

	//scale matrix
	//xyz	scale parm in xyz
	Mat4 scale_matrix(const Vec3& xyz);

	//perspective projection matrix
	//aspect	aspect-- 16:9 or 4:3 usually
	Mat4 perspective_projection_matrix(float aspect,
		float theta, float znear, float zfar);

	Mat4 orthographic_projection_matrix(float xmin, float xmax,
		float ymin, float ymax, float znear, float zfar);

	Mat4 look_at_matrix(const Vec3& eye,
		const Vec3& target, const Vec3& up);

	Mat4 look_at_matrix_by_look_at_dir(
		const Vec3& eye, const Vec3& look_at, const Vec3& up);

	Mat4 inverse_matrix(const Mat4&);
}

