#pragma once

#include "CommonUtility.h"
#include "NodeResource.h"
#include "Mesh.h"
#include "Geometry.h"

namespace Necromancer{
	class NodeGeometryResource:public NodeResource{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(GeometryPtr, geometry);
	public:
		NodeGeometryResource(const GeometryPtr&);
		virtual ~NodeGeometryResource();

		virtual SceneNodeType get_node_type();
		virtual void update(SceneNode*);
	private:
	};
}

