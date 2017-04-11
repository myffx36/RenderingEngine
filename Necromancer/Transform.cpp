#include "stdafx.h"
#include "Transform.h"

namespace Necromancer{
	Translation::Translation(const Vec3& vec)
		:m_vec(vec)
	{

	}

	void Translation::apply_to_translation(Translation& translation){
		Vec3 vec = m_vec + translation.vec();
		translation.set_vec(vec);
	}

	Rotation::Rotation(const Quaternion& quternion)
		:m_quaternion(quternion)
	{

	}

	void Rotation::apply_to_rotation(Rotation& rotation){
		auto quanternion = rotation.quaternion();
		auto x = m_quaternion * quanternion;
		rotation.set_quaternion(x);
	}

	void Rotation::apply_to_translation(Translation& translation){
		auto vec = translation.vec();
		vec = m_quaternion.rotate_vec3(vec);
		translation.set_vec(vec);
	}

	Scale::Scale(const Vec3& vec)
		:m_vec(vec)
	{
	}

	void Scale::apply_to_scale(Scale& scale){
		auto vec = scale.vec();
		vec = vec * m_vec;
		scale.set_vec(vec);
	}

	void Scale::apply_to_translation(Translation& translation){
		auto vec = translation.vec();
		vec = vec * m_vec;
		translation.set_vec(vec);
	}

	Transform::Transform(const Transform& others)
		:m_translation(others.translation()),
		m_rotation(others.rotation()),
		m_scale(others.scale())
	{
		recompute_matrix();
	}

	Transform::Transform()
		:m_translation(Vec3(0.0f, 0.0f, 0.0f)),
		m_rotation(Quaternion()),
		m_scale(Vec3(1.0f, 1.0f, 1.0f))
	{
		recompute_matrix();
	}

	Transform::~Transform(){
	}

	void Transform::apply_transform(const Transform& transform){
		auto scale = transform.scale();
		auto rotation = transform.rotation();
		auto translation = transform.translation();
		scale.apply_to_scale(m_scale);
		scale.apply_to_translation(m_translation);
		rotation.apply_to_rotation(m_rotation);
		rotation.apply_to_translation(m_translation);
		translation.apply_to_translation(m_translation);
	}

	void Transform::recompute_matrix(){
		Mat4 matrix = 
			scale_matrix(m_scale.vec()) ;
		Mat4 rotation_matrix = 
			m_rotation.quaternion().convert_to_matrix4x4();
		Mat4 translation_matrix = 
			translate_matrix(m_translation.vec());
		matrix = matrix * rotation_matrix * translation_matrix;
		m_matrix = matrix;
	}

	Transform& Transform::operator=(const Transform& transform){
		set_translation(transform.translation());
		set_rotation(transform.rotation());
		set_scale(transform.scale());

		return *this;
	}

	void Transform::set_translation(F32 x, F32 y, F32 z){
		set_translation(Translation(Vec3(x, y, z)));
	}

	void Transform::set_rotation(F32 x, F32 y, F32 z, F32 w){
		set_rotation(Rotation(Quaternion(x, y, z, w)));
	}

	void Transform::set_scale(F32 x, F32 y, F32 z){
		set_scale(Scale(Vec3(x, y, z)));
	}
}