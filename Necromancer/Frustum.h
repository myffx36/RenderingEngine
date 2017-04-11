#pragma once

#include "CommonUtility.h"
#include "BasicMath.h"

namespace Necromancer{
	class Frustum{
		DEF_DATA_MEMBER_AND_GETTER(Vec4, top);
		DEF_DATA_MEMBER_AND_GETTER(Vec4, down); 
		DEF_DATA_MEMBER_AND_GETTER(Vec4, n); 
		DEF_DATA_MEMBER_AND_GETTER(Vec4, f); 
		DEF_DATA_MEMBER_AND_GETTER(Vec4, left);
		DEF_DATA_MEMBER_AND_GETTER(Vec4, right);
	public:
		Frustum(const Mat4& mat);
	};
}

