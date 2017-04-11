#include "stdafx.h"
#include "SkeletonInstance.h"

#include "SceneNode.h"
#include "Exception.h"

namespace Necromancer{
	void SkeletonInstance::set_matrix(U32 i, const Mat4& matrix){
		m_matrices[i] = matrix;
	}

	Mat4 SkeletonInstance::get_matrix(U32 i)const{
		return m_matrices[i];
	}

	SkeletonInstance::SkeletonInstance(
		const SkeletonPtr& skeleton)
		:m_skeleton(skeleton)
	{
		m_bone_num = skeleton->bone_num();
		m_current_time = 0.0f;
		m_state = NECROMANCER_SI_STATUS_NORMAL;
	}

	void SkeletonInstance::update(F32 dlt_time){
		switch (m_state){
		case NECROMANCER_SI_STATUS_PAUSE:
			break;
		case NECROMANCER_SI_STATUS_NORMAL:
			m_current_time += dlt_time;
			do_update();
			break;
		case NECROMANCER_SI_STATUS_INACTIVE:
			m_current_time += dlt_time;
			break;
		}
	}

	void SkeletonInstance::do_update(){
		for (U32 i = 0; i < m_skeleton->bone_num(); ++i){
			m_matrices[i] = 
				Mat4(0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f);
		}
		for (auto iter = m_animation_instance_map.begin();
			iter != m_animation_instance_map.end(); ++iter)
		{
			iter->second->update(m_current_time);
			for (U32 i = 0; i < m_skeleton->bone_num(); ++i){
				m_matrices[i] = m_matrices[i] +
					iter->second->get_matrix(i) * 
					iter->second->weight();
			}
		}
	}

	void SkeletonInstance::attach_animation_instance(
		const AnimationInstancePtr& animation_instance)
	{
		m_animation_instance_map[animation_instance->name()] =
			animation_instance;
	}

	void SkeletonInstance::detach_animation_instance(
		const String& name)
	{
		auto iter = m_animation_instance_map.find(name);
		if (iter != m_animation_instance_map.end()){
			m_animation_instance_map.erase(iter);
		}
		else{
			//someting wrong
			//no animation named this attached
			throw Exception("no animation named \"" + name + "\" attached");
		}
	}
}