#pragma once

#include "Material.h"
#include "SubScene.h"
#include "Skeleton.h"
#include "Animation.h"

namespace Necromancer{
	SubScenePtr create_sub_scene_from_fbx_file(const char*);
	AnimationPtr create_animation_from_fbx_file(const char*);

	class NECFBXFile{
		DEF_DATA_MEMBER_AND_GETTER(SubScenePtr, sub_scene);
		DEF_DATA_MEMBER_AND_GETTER(AnimationPtr, animation);
	public:
		NECFBXFile(const char*);
		~NECFBXFile();
	private:
	};
}

