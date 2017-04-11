#include "stdafx.h"
#include "NodeGeometryResource.h"

namespace Necromancer{
	NodeGeometryResource::NodeGeometryResource(
		const GeometryPtr& geometry)
		:m_geometry(geometry)
	{
	}

	NodeGeometryResource::~NodeGeometryResource(){
	}

	SceneNodeType NodeGeometryResource::get_node_type(){
		return NM_GEOMETRY_NODE;
	}

	void NodeGeometryResource::update(SceneNode* node){
		m_geometry->set_world(node->global());
	}
}