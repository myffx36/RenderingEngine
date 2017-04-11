#include "stdafx.h"
#include "NodeLightResource.h"

namespace Necromancer{
	NodeLightResource::NodeLightResource(const Light& light)
		:m_light(light)
	{

	}

	NodeLightResource::~NodeLightResource(){

	}

	SceneNodeType NodeLightResource::get_node_type(){
		return NM_LIGHT_NODE;
	}

	void NodeLightResource::update(SceneNode* node){
		Vec4 origin(0.0f, 0.0f, 0.0f, 1.0f);
		Vec4 position = origin * node->global();
		position = position / position.w;
		Vec4 default_direction(0.0f, 0.0f, -1.0f, 0.0f);
		Vec4 direction = default_direction * node->global();
		m_light.set_position(position);
		m_light.set_direction(direction);
	}
}