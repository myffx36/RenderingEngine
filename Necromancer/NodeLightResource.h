#pragma once

#include "NodeResource.h"
#include "CommonUtility.h"
#include "Light.h"

namespace Necromancer{
	class NodeLightResource:public NodeResource{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Light, light);
	public:
		NodeLightResource(const Light&);
		virtual ~NodeLightResource();

		virtual SceneNodeType get_node_type();
		virtual void update(SceneNode*);
	private:
	};
}

