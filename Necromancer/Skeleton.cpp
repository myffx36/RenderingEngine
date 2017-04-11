#include "stdafx.h"
#include "Skeleton.h"

#include "Exception.h"


namespace Necromancer{
	Skeleton::~Skeleton(){

	}

	U32 Skeleton::bone_num() const{
		return static_cast<U32>(m_bone_name.size());
	}

	void Skeleton::add_bone(const String& name){
		m_bone_name.push_back(name);
	}

	U32 Skeleton::get_bone_index(const String& name){
		U32 r = 0;
		for (r = 0; r < m_bone_name.size(); ++r){
			if (m_bone_name[r] == name)
				return r;
		}
		//TODO: exception
		throw Exception("no bone named " + name);
		return r;
	}
}