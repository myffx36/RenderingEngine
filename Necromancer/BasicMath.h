#pragma once

#include "CommonUtility.h"

namespace Necromancer{

	/* BasicMath.h
	 * ������������������Ԫ���Ļ���ʵ��
	 * ����������union�ķ�ʽ��ʹ��֧������vec.x�����ķ��ʷ�ʽ
	 * ͬʱ�������˸�����ͼ��ѧ����ʹ�õıȽ϶�ľ���
	 * ��͸��ͶӰ�����Լ����ֱ任����
	 */

	//��ά����������

	//��ά����
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

	//���
	const F32 dot(const Vec2&, const Vec2&);

	//����
	const F32 length(const Vec2&);

	//��ά�������ֽ���


	//��ά��������
	
	//��ά����
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

	//���
	const F32 dot(const Vec3&, const Vec3&);

	//���
	const Vec3 cross(const Vec3&, const Vec3&);

	//���㳤��
	const F32 length(const Vec3&);

	//��Ϊ��λ����
	const Vec3 normalize(const Vec3&);

	//��ά������������


	//��ά��������

	//��ά����
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

	//���
	const F32 dot(const Vec4&, const Vec4&);

	//����
	const F32 length(const Vec4&);

	//��׼��
	const Vec4 normalize(const Vec4&);

	//��ά������������


	//��ά��������

	//��ά����
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


	//��ά������������



	//��ά��������

	//��ά����
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


	//��ά������������


	
	//��Ԫ������

	//��Ԫ��
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

	//��Ԫ��ȡ��
	Quaternion inverse_quaternion(const Quaternion&);

	//����ת����ת��Ϊ��Ԫ��
	Quaternion mat3_to_quaternion(const Mat3&);

	//����ת����ת��Ϊ��Ԫ��
	Quaternion mat4_to_quaternion(const Mat4&);

	//����-�ǵ���ʽ������Ԫ��
	//���� axis ��ת��
	//���� theta ��ת��(�Ƕȱ�ʾ) 
	Quaternion make_quanternion(const Vec3& axis, F32 theta);

	//��ŷ���ǹ�����Ԫ��
	Quaternion make_quanternion_from_euler(const Vec3&);

	//��lerp�ķ�ʽ�����Ԫ��(�ٶȿ� Ч����slerp��)
	Quaternion blend_lerp(F32 b, 
		const Quaternion& q1, const Quaternion& q2);

	//��slerp�ķ�ʽ�����Ԫ��(�ٶ��� Ч����lerp��)
	Quaternion blend_slerp(F32 b, 
		const Quaternion& q1, const Quaternion& q2);


	//��Ԫ����������


	
	//���þ��������

	//ƽ�ƾ���
	//����x  ��x�����ϵ�ƫ����
	//����y  ��y�����ϵ�ƫ����
	//����z  ��z�����ϵ�ƫ����
	Mat4 translate_matrix(F32 x, F32 y, F32 z);

	//ƽ�ƾ���
	//����xyz  ��xyz�����ϸ��Ե�ƫ����
	Mat4 translate_matrix(const Vec3& xyz);

	//��ת����
	//����axis_x  ��ת���x����
	//����axis_y  ��ת���y����
	//����axis_z  ��ת���z����
	//����theta   ��ת�Ƕ�(�Ƕȱ�ʾ)
	//ע�Ᵽ֤��ת��Ϊ��λ����
	Mat4 rotate_matrix(F32 axis_x, F32 axis_y, F32 axis_z, F32 theta);

	//��ת����
	//����axis   ��ת��
	//����theta  ��ת��(�Ƕȱ�ʾ)
	//ע�Ᵽ֤��ת��Ϊ��λ����
	Mat4 rotate_matrix(const Vec3& axis, F32 theta);

	//��ת����(��һ������ת����һ�������ķ���)
	//����from   ��ʼ����
	//����to     ��ֹ����
	//ע�Ᵽ֤�����������ǵ�λ����
	Mat4 rotate_matrix_vec_to_vec(const Vec3& from, const Vec3& to);

	//���ž���
	//����x		��x�����ϵ����Ų���
	//����y		��y�����ϵ����Ų���
	//����z		��z�����ϵ����Ų���
	Mat4 scale_matrix(F32 x, F32 y, F32 z);

	//���ž���
	//����xyz	��xyz���������ϵ����Ų���
	//��ͬ�ڵ������º���  scale_matrix(xyz.x, xyz.y, xyz.z)
	Mat4 scale_matrix(const Vec3& xyz);

	//͸��ͶӰ����
	//����aspect		�߿��
	//����theta		�ӽǿ��
	//����znear		������ƽ���λ��
	//����zfar		Զ����ƽ���λ��
	Mat4 perspective_projection_matrix(F32 aspect, 
		F32 theta, F32 znear, F32 zfar);

	//ƽ��ͶӰ����
	//����xmin		x������Сֵ
	//����xmax		x�������ֵ
	//����ymin		y������Сֵ
	//����ymax		y�������ֵ
	//����zmin		z������Сֵ
	//����zmax		z�������ֵ
	Mat4 orthographic_projection_matrix(F32 xmin, F32 xmax,
		F32 ymin, F32 ymax, F32 znear, F32 zfar);

	//lookat����
	//����eye		�۲��λ��
	//����target	�۲��Ŀ��
	//����up		��������
	Mat4 look_at_matrix(const Vec3& eye, 
		const Vec3& target, const Vec3& up);

	//lookat����
	//����eye		�۲��λ��
	//����look_at	�۲�ķ���
	//����up		��������
	Mat4 look_at_matrix_by_look_at_dir(
		const Vec3& eye, const Vec3& look_at, const Vec3& up);

	//�������
	Mat4 inverse_matrix(const Mat4&);

	//���þ�����������
}

