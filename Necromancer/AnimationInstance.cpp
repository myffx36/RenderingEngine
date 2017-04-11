#include "stdafx.h"
#include "AnimationInstance.h"

namespace Necromancer{
	AnimationInstance::AnimationInstance(
		const AnimationPtr& animation,
		bool start_eased_in, I32 max_cycles)
		:m_animation(animation), m_name(animation->name())
	{
		m_matrices.resize(animation->bone_number());
	}

	AnimationInstance::~AnimationInstance(){

	}

	void AnimationInstance::set_weight(F32 weight){
	}

	F32 AnimationInstance::weight() const{
		return 1.0f;
	}

	void AnimationInstance::ease_in(F32 duration){
	}

	void AnimationInstance::ease_out(F32 duration){
	}

	void AnimationInstance::set_play_back_speed(F32 speed){
	}

	void AnimationInstance::update(F32 current_time){
		F32 duration = m_animation->duration();
		F32 dlt_time = current_time;
		while (dlt_time > duration){
			dlt_time -= duration;
		}

		U32 n = m_animation->frame_number();
		U32 prev, next;
		for (U32 i = 0; i < n; ++i){
			F32 cur_frame_time = m_animation->get_frame_time(i);
			if (cur_frame_time > dlt_time){
				prev = i - 1;
				next = i;
				break;
			}
		}

		F32 prev_frame_time = m_animation->get_frame_time(prev);
		F32 next_frame_time = m_animation->get_frame_time(next);
		F32 frame_dlt_time = next_frame_time - prev_frame_time;
		F32 past_time = dlt_time - prev_frame_time;
		F32 param = past_time / frame_dlt_time;

		for (U32 i = 0; i < m_animation->bone_number(); ++i){
			m_matrices[i] = m_animation->get_matrix(i, prev) * (1 - param) +
				m_animation->get_matrix(i, next) * param;
			//m_matrices[i] = m_animation->get_matrix(i, prev);
		}
	}

	Mat4 AnimationInstance::get_matrix(U32 i) const{
		return m_matrices[i];
	}
}