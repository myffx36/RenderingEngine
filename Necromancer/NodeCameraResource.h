#pragma once

#include "NodeResource.h"
#include "Camera.h"
#include "CommonUtility.h"

namespace Necromancer{
	class NodeCameraResource:public NodeResource{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(CameraPtr, camera);
	public:
		NodeCameraResource(const CameraPtr&);
		virtual ~NodeCameraResource();

		virtual SceneNodeType get_node_type() = 0;
		virtual void update(SceneNode*);
	private:
	};
}

