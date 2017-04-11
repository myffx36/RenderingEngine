#pragma once

#include "CommonUtility.h"
#include "BasicMath.h"
#include "Container.hpp"
#include "Transform.h"

namespace Necromancer{
	enum SceneNodeType{
		NM_SCENE_NODE,
		NM_LIGHT_NODE,
		NM_GEOMETRY_NODE,
		NM_CAMERA_NODE,
		NM_SKELETON_ROOT_NODE,
		NM_SKELETON_NODE
	};
	class NodeResource;
	class BoundingVolume;
	class SceneNode{
		static const U32 ks_max_children_number = 32;
	public:
		typedef Vector<SceneNode*>::Type ChildrenArray;
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Mat4, local);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Mat4, global);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(SceneNode, parent);
		DEF_DATA_MEMBER_AND_GETTER(SceneNodeType, node_type);
		DEF_DATA_MEMBER_AND_GETTER(ChildrenArray, children);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Transform, local_transform);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Transform, global_transform);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(BoundingVolume, bounding_volume);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, name);
	public:
		SceneNode(SceneNode* parent = nullptr,
			SceneNodeType type = NM_SCENE_NODE);

		virtual ~SceneNode();

		void attach_child(SceneNode*);
		void detach_child(SceneNode*);

		U32 children_number() const;

		void update();

		NodeResource* node_resource();
		void set_node_resource(NodeResource*);
	private:
		NodeResource* m_node_resource;
	};
}

