#pragma once

#include "CommonUtility.h"
#include "Container.hpp"

namespace Necromancer{
	class Skeleton{
	public:
		~Skeleton();

		U32 bone_num() const;

		void add_bone(const String&);
		U32 get_bone_index(const String&);
	private:
		Vector<String>::Type m_bone_name;
	};
	typedef SmartPtr<Skeleton>::Type SkeletonPtr;
}

