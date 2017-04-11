#pragma once

#include "CommonUtility.h"

namespace Necromancer{

	/* BasicMath.h
	 * 包含了向量、矩阵、四元数的基本实现
	 * 向量采用了union的方式来使其支持类似vec.x这样的访问方式
	 * 同时还包含了各种在图形学当中使用的比较多的矩阵
	 * 如透视投影矩阵以及各种变换矩阵
	 */

	//二维向量的声明

	//二维向量
	union Vec2{
		struct{
			F32 x, y;
		};

		struct{
			F32 data[2];
		};

		Vec2(){}
		Vec2(F32, F32);

		F32& operator[](U32 i);
		const F32& operator[](U32 i) const;

		const Vec2 operator+(const Vec2&) const;
		const Vec2 operator-(const Vec2&) const;
		const Vec2 operator*(const Vec2&) const;
		const Vec2 operator/(const Vec2&) const;
		const Vec2 operator*(const F32) const;
		const Vec2 operator/(const F32) const;
	};

	const Vec2 operator*(const F32, const Vec2&);

	//点积
	const F32 dot(const Vec2&, const Vec2&);

	//长度
	const F32 length(const Vec2&);

	//二维向量部分结束


	//三维向量声明
	
	//三维向量
	union  Vec3{
		struct{
			F32 x, y, z;
		};
		struct{
			F32 data[3];
		};
		struct{
			Vec2 xy;
			F32 z;
		};
		struct{
			F32 x;
			Vec2 yz;
		};

		Vec3(){}
		Vec3(F32, F32, F32);
		Vec3(const Vec2&, F32);
		Vec3(F32, const Vec2&);

		F32& operator[](U32 i);
		const F32& operator[](U32 i)const;

		const Vec3 operator+(const Vec3&) const;
		const Vec3 operator-(const Vec3&) const;
		const Vec3 operator*(const Vec3&) const;
		const Vec3 operator/(const Vec3&) const;
		const Vec3 operator*(const F32) const;
		const Vec3 operator/(const F32) const;
	};

	const Vec3 operator*(const F32, const Vec3&);

	//点积
	const F32 dot(const Vec3&, const Vec3&);

	//叉积
	const Vec3 cross(const Vec3&, const Vec3&);

	//计算长度
	const F32 length(const Vec3&);

	//化为单位向量
	const Vec3 normalize(const Vec3&);

	//三维向量声明结束


	//四维向量声明

	//四维向量
	union Vec4{
		struct{
			F32 x, y, z, w;
		};
		struct{
			F32 data[4];
		};
		struct{
			Vec3 xyz;
			F32 w;
		};
		struct{
			F32 x;
			Vec3 yzw;
		};
		struct{
			Vec2 xy;
			Vec2 zw;
		};

		Vec4(){}
		Vec4(F32, F32, F32, F32);
		Vec4(const Vec3&, F32);
		Vec4(F32, const Vec3&);
		Vec4(const Vec2&, const Vec2&);

		F32& operator[](U32 i);
		const F32& operator[](U32 i)const;

		const Vec4 operator+(const Vec4&) const;
		const Vec4 operator-(const Vec4&) const;
		const Vec4 operator*(const Vec4&) const;
		const Vec4 operator/(const Vec4&) const;
		const Vec4 operator*(const F32) const;
		const Vec4 operator/(const F32) const;
	};

	const Vec4 operator*(const F32, const Vec4&);

	//点积
	const F32 dot(const Vec4&, const Vec4&);

	//长度
	const F32 length(const Vec4&);

	//标准化
	const Vec4 normalize(const Vec4&);

	//四维向量声明结束


	//三维矩阵声明

	//三维矩阵
	class Mat3{
	public:
		Mat3();
		Mat3(F32, F32, F32,
			F32, F32, F32,
			F32, F32, F32);
		Mat3(const Vec3&,
			const Vec3&, 
			const Vec3&);

		Vec3& operator[](U32 i);
		const Vec3& operator[](U32 i)const;

		const Mat3 operator+(const Mat3&) const;
		const Mat3 operator-(const Mat3&) const;
		const Mat3 operator*(const Mat3&) const;
		const Mat3 operator*(const F32) const;
		const Mat3 operator/(const F32) const;
	private:
		Vec3 m0, m1, m2;
	};

	const Vec3 operator*(const Vec3&, const Mat3&);


	//三维矩阵声明结束



	//四维矩阵声明

	//四维矩阵
	class Mat4;

	class Mat4{
	public:
		Mat4(F32, F32, F32, F32,
			F32, F32, F32, F32,
			F32, F32, F32, F32,
			F32, F32, F32, F32);
		Mat4(const Vec4&,
			const Vec4&,
			const Vec4&,
			const Vec4&);
		Mat4();

		Vec4& operator[](U32 i);
		const Vec4& operator[](U32 i)const;

		const Mat4 operator+(const Mat4&)const;
		const Mat4 operator-(const Mat4&)const;
		const Mat4 operator*(const Mat4&)const;
		const Mat4 operator*(const F32)const;
		const Mat4 operator/(const F32)const;
	private:
		Vec4 m0, m1, m2, m3;
	};

	const Vec4 operator*(const Vec4&, const Mat4&);


	//四维矩阵声明结束


	
	//四元数声明

	//四元数
	class Quaternion{
	public:
		Quaternion();
		Quaternion(const Vec4&);
		Quaternion(F32, F32, F32, F32);
		~Quaternion();

		const Quaternion operator*(const Quaternion&) const;
		Vec4 rotate_vec4(const Vec4&);
		Vec3 rotate_vec3(const Vec3&);
		Mat3 convert_to_matrix3x3() const;
		Mat4 convert_to_matrix4x4() const;
		F32& operator[](U32 i);
		F32 length() const;
		const F32& operator[](U32 i) const;

		Vec3 get_axis();
		float get_theta();
	private:
		Vec4 m_data;
	private:
	};

	//四元数取反
	Quaternion inverse_quaternion(const Quaternion&);

	//将旋转矩阵转换为四元数
	Quaternion mat3_to_quaternion(const Mat3&);

	//将旋转矩阵转换为四元数
	Quaternion mat4_to_quaternion(const Mat4&);

	//以轴-角的形式构造四元数
	//参数 axis 旋转轴
	//参数 theta 旋转角(角度表示) 
	Quaternion make_quanternion(const Vec3& axis, F32 theta);

	//从欧拉角构造四元数
	Quaternion make_quanternion_from_euler(const Vec3&);

	//以lerp的方式混合四元数(速度快 效果比slerp差)
	Quaternion blend_lerp(F32 b, 
		const Quaternion& q1, const Quaternion& q2);

	//以slerp的方式混合四元数(速度慢 效果比lerp好)
	Quaternion blend_slerp(F32 b, 
		const Quaternion& q1, const Quaternion& q2);


	//四元数声明结束


	
	//常用矩阵的声明

	//平移矩阵
	//参数x  在x方向上的偏移量
	//参数y  在y方向上的偏移量
	//参数z  在z方向上的偏移量
	Mat4 translate_matrix(F32 x, F32 y, F32 z);

	//平移矩阵
	//参数xyz  在xyz方向上各自的偏移量
	Mat4 translate_matrix(const Vec3& xyz);

	//旋转矩阵
	//参数axis_x  旋转轴的x分量
	//参数axis_y  旋转轴的y分量
	//参数axis_z  旋转轴的z分量
	//参数theta   旋转角度(角度表示)
	//注意保证旋转轴为单位向量
	Mat4 rotate_matrix(F32 axis_x, F32 axis_y, F32 axis_z, F32 theta);

	//旋转矩阵
	//参数axis   旋转轴
	//参数theta  旋转角(角度表示)
	//注意保证旋转轴为单位向量
	Mat4 rotate_matrix(const Vec3& axis, F32 theta);

	//旋转矩阵(从一个向量转到另一个向量的方向)
	//参数from   起始向量
	//参数to     终止向量
	//注意保证两个参数都是单位向量
	Mat4 rotate_matrix_vec_to_vec(const Vec3& from, const Vec3& to);

	//缩放矩阵
	//参数x		在x方向上的缩放参数
	//参数y		在y方向上的缩放参数
	//参数z		在z方向上的缩放参数
	Mat4 scale_matrix(F32 x, F32 y, F32 z);

	//缩放矩阵
	//参数xyz	在xyz三个方向上的缩放参数
	//等同于调用如下函数  scale_matrix(xyz.x, xyz.y, xyz.z)
	Mat4 scale_matrix(const Vec3& xyz);

	//透视投影矩阵
	//参数aspect		高宽比
	//参数theta		视角宽度
	//参数znear		近剪裁平面的位置
	//参数zfar		远剪裁平面的位置
	Mat4 perspective_projection_matrix(F32 aspect, 
		F32 theta, F32 znear, F32 zfar);

	//平行投影矩阵
	//参数xmin		x方向最小值
	//参数xmax		x方向最大值
	//参数ymin		y方向最小值
	//参数ymax		y方向最大值
	//参数zmin		z方向最小值
	//参数zmax		z方向最大值
	Mat4 orthographic_projection_matrix(F32 xmin, F32 xmax,
		F32 ymin, F32 ymax, F32 znear, F32 zfar);

	//lookat矩阵
	//参数eye		观察的位置
	//参数target	观察的目标
	//参数up		朝上向量
	Mat4 look_at_matrix(const Vec3& eye, 
		const Vec3& target, const Vec3& up);

	//lookat矩阵
	//参数eye		观察的位置
	//参数look_at	观察的方向
	//参数up		朝上向量
	Mat4 look_at_matrix_by_look_at_dir(
		const Vec3& eye, const Vec3& look_at, const Vec3& up);

	//求逆矩阵
	Mat4 inverse_matrix(const Mat4&);

	//常用矩阵声明结束
}

