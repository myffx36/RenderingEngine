#pragma once

#include "CommonUtility.h"
#include "Container.hpp"
#include "BasicMath.h"

class hkaAnimation;
class hkaAnimationBinding;

namespace Necromancer{
	class Animation{
		DEF_DATA_MEMBER_AND_GETTER(U32, bone_number);
		DEF_DATA_MEMBER_AND_GETTER(U32, frame_number);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, name);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(F32, duration);
	public:
		Animation(const String& name,
			U32 bone_number, U32 frame_number);
		virtual ~Animation();

		void set_matrix(U32 bone_number, U32 frame_number, const Mat4&);
		Mat4 get_matrix(U32 bone_number, U32 frame_number) const;

		F32 get_frame_time(U32 i) const;
		void set_frame_time(U32, F32);

	private:
		Vector<Vector<Mat4>::Type>::Type m_matrices;
		Vector<F32>::Type m_frame_time;
	};

	typedef SmartPtr<Animation>::Type AnimationPtr;
}

