#pragma once

#include "Container.hpp"
#include "CommonUtility.h"
#include "Animation.h"

class hkaDefaultAnimationControl;

namespace Necromancer{
	class AnimationInstance{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, name);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(AnimationPtr, animation);
	public:
		AnimationInstance(const AnimationPtr&,
			bool start_eased_in = true, I32 max_cycles = -1);

		~AnimationInstance();

		void set_weight(F32 weight);
		F32 weight() const;

		void ease_in(F32 duration);
		void ease_out(F32 duration);

		void set_play_back_speed(F32);

		void update(F32 current_time);

		Mat4 get_matrix(U32 i) const;
	private:
		Vector<Mat4>::Type m_matrices;
	};

	typedef SmartPtr<AnimationInstance>::Type AnimationInstancePtr;
}

