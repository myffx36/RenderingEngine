#pragma once

#include "BasicMath.h"
#include "CommonUtility.h"

namespace Necromancer{
	class Translation{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec3, vec);
	public:
		Translation(const Vec3&);

		void apply_to_translation(Translation&);
	private:
	};

	class Rotation{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Quaternion, quaternion);
	public:
		Rotation(const Quaternion&);

		void apply_to_translation(Translation&);
		void apply_to_rotation(Rotation&);
	private:
	};

	class Scale{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec3, vec);
	public:
		Scale(const Vec3&);

		void apply_to_translation(Translation&);
		void apply_to_scale(Scale&);
	private:
	};

	class Transform{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Translation, translation);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Rotation, rotation);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Scale, scale);
		DEF_DATA_MEMBER_AND_GETTER(Mat4, matrix);
	public:
		Transform();
		~Transform();
		Transform(const Transform&);

		void apply_transform(const Transform& transform);

		void set_translation(F32, F32, F32);
		void set_rotation(F32, F32, F32, F32);
		void set_scale(F32, F32, F32);

		Transform& operator=(const Transform&);
		void recompute_matrix();
	private:
	};
}

