#pragma once

#include "Container.hpp"
#include "CommonUtility.h"
#include "Transform.h"

namespace Necromancer{
	class SceneNode;
	class Animation;
	class SubScene{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(SceneNode, root_node);
		//DEF_DATA_MEMBER_AND_GETTER_SETTER(SkeletonPtr, skeleton);
	public:
		SubScene();
		~SubScene();

		void enable_debug_skeleton();

		void set_transform(const Transform&);
		Transform transform() const;
	private:
		static const U32 max_node_size = 128;
	};

	typedef SmartPtr<SubScene>::Type SubScenePtr;
}

