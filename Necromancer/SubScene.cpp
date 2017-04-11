#include "stdafx.h"
#include "SubScene.h"

#include "Necromancer.h"
//#include "AnimationEngine.h"
//#include "AnimationPipeline.h"
#include "NodeGeometryResource.h"

namespace Necromancer{
	SubScene::SubScene()
	{
	}

	SubScene::~SubScene(){
		if(m_root_node != nullptr){
			if(m_root_node->parent() == nullptr){
				delete m_root_node;
			}
		}
	}

	void SubScene::enable_debug_skeleton(){
	}

	Transform SubScene::transform() const{
		return m_root_node->global_transform();
	}

	void SubScene::set_transform(const Transform& transform){
		m_root_node->set_local_transform(transform);
	}
}