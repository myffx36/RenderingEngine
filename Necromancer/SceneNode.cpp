#include "stdafx.h"
#include "SceneNode.h"
#include "NodeResource.h"
#include "BoundingVolume.h"
#include "Necromancer.h"
#include "RenderingEngine.h"
#include "NodeObserverManager.h"

namespace Necromancer{
	SceneNode::SceneNode(SceneNode* parent, SceneNodeType type){
		m_parent = parent;
		m_node_type = type;

		if(parent != nullptr)
			parent->attach_child(this);

		m_local = translate_matrix(0.0f, 0.0f, 0.0f);

		m_bounding_volume = new BoundingVolume();

		m_node_resource = nullptr;
	}

	SceneNode::~SceneNode(){
		if(m_parent)
			m_parent->detach_child(this);

		Vector<SceneNode*>::Type nodes_to_delete;

		for(U32 i = 0;i < m_children.size();++ i){
			nodes_to_delete.push_back(m_children[i]);
		}

		for (U32 i = 0; i < nodes_to_delete.size(); ++i){
			delete nodes_to_delete[i];
		}

		if(m_node_resource) delete m_node_resource;
		if(m_bounding_volume) delete m_bounding_volume;
	}

	void SceneNode::attach_child(SceneNode* child){
		m_children.push_back(child);
		child->set_parent(this);
	}

	void SceneNode::detach_child(SceneNode* child){
		auto iter = std::find(m_children.begin(),
			m_children.end(), child);
		if (iter != m_children.end()){
			m_children.erase(iter);
		}
	}

	void SceneNode::update(){
		if(m_node_resource)
			m_node_resource->before_update(this);

		//update transform
		m_local_transform.recompute_matrix();
		m_local = m_local_transform.matrix();
		if(m_parent != nullptr){
			auto parent_global_transform = 
				m_parent->global_transform();
			m_global_transform = m_local_transform;
			m_global_transform.apply_transform(
				parent_global_transform);
		}else{
			m_global_transform = m_local_transform;
		}
		m_global_transform.recompute_matrix();
		m_global = m_global_transform.matrix();

		if(m_node_resource)m_node_resource->update(this);

		for(U32 i = 0;i < m_children.size();++ i){
			m_children[i]->update();
		}

		m_bounding_volume->update_bv(this);
		if(m_node_resource)m_node_resource->after_update(this);


		Necromancer* necromancer = Necromancer::get_instance();
		RenderingEngine* re = necromancer->rendering_engine();
		NodeObserverManager* observer_manager = re->node_observer_manager();
		observer_manager->notify_update(this);
	}

	NodeResource* SceneNode::node_resource(){
		return m_node_resource;
	}

	void SceneNode::set_node_resource(NodeResource* node_resource){
		if(m_node_resource != nullptr){
			delete m_node_resource;
		}
		m_node_resource = node_resource;
		m_node_type = node_resource->get_node_type();
	}

	U32 SceneNode::children_number() const{
		return static_cast<U32>(m_children.size());
	}
}