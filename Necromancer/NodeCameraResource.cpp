#include "stdafx.h"
#include "NodeCameraResource.h"

namespace Necromancer{
	NodeCameraResource::NodeCameraResource(const CameraPtr& camera)
		:m_camera(camera)
	{

	}

	NodeCameraResource::~NodeCameraResource(){

	}

	SceneNodeType NodeCameraResource::get_node_type(){
		return NM_CAMERA_NODE;
	}

	void NodeCameraResource::update(SceneNode* node){
		auto global_matrix = node->global();
		m_camera->set_eye(global_matrix[3].xyz);

		Mat3 global_matrix3x3;
		for(int i = 0;i < 3;++ i){
			for(int j = 0;j < 3;++ j){
				global_matrix3x3[i][j] = global_matrix[i][j];
			}
		}

		auto look_at = 
			Vec3(0.0f, 0.0f, 1.0f) * global_matrix3x3;
		auto up = 
			Vec3(0.0f, 1.0f, 0.0f) * global_matrix3x3;
		m_camera->set_look_at(look_at);
		m_camera->set_up(up);
		m_camera->update_view_matrix();
	}
}