#pragma once

#include "SceneNode.h"

namespace Necromancer{
	class NodeResource{
	public:
		virtual ~NodeResource();

		virtual SceneNodeType get_node_type() = 0;
		virtual void before_update(SceneNode*);
		virtual void update(SceneNode*);
		virtual void after_update(SceneNode*);
	private:
	};
}

