#pragma once

#include "CommonUtility.h"
#include "Container.hpp"
#include "BasicMath.h"
#include "NecromancerLimit.h"
#include "AnimationInstance.h"
#include "Skeleton.h"

namespace Necromancer{
	class SceneNode;
	enum SkeletonInstanceState
	{
		NECROMANCER_SI_STATUS_NORMAL,
		NECROMANCER_SI_STATUS_PAUSE,
		NECROMANCER_SI_STATUS_INACTIVE
	};
	class SkeletonInstance{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, name);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(SkeletonPtr, skeleton);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(SceneNode, skeleton_root_node);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(F32, current_time);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(SkeletonInstanceState, state);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, bone_num);
	public:
		SkeletonInstance(const SkeletonPtr&);
		void set_matrix(U32 i, const Mat4&);
		Mat4 get_matrix(U32 i) const;

		void update(F32 dlt_time);

		void attach_animation_instance(const AnimationInstancePtr&);
		void detach_animation_instance(const String&);
	private:
		void do_update();
	private:
		Mat4 m_matrices[ks_skeleton_max_matrix_num];
		Map<String, AnimationInstancePtr>::Type 
			m_animation_instance_map;
	};

	typedef SmartPtr<SkeletonInstance>::Type SkeletonInstancePtr;
}

