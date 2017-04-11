#include "stdafx.h"
#include "Scene.h"

#include "SceneNode.h"
#include "SubScene.h"
#include "Frustum.h"
#include "NodeGeometryResource.h"
#include "NodeLightResource.h"

namespace Necromancer{
	namespace{
		const U32 max_result_size = 128;
		template<typename T, typename Lam>
		U32 find_all_node(SceneNode* node, SceneNodeType type, T** result, Lam lam){
			std::stack<SceneNode*> stack;
			stack.push(node);
			U32 n = 0;

			while (!stack.empty()){
				SceneNode* cur_node = stack.top();
				stack.pop();

				auto children = cur_node->children();
				U32 children_number = cur_node->children_number();
				for (U32 i = 0; i < children_number; ++i){
					stack.push(children[i]);
				}

				if ((cur_node->node_type() == type) &&
					(lam((T*) cur_node)) &&
					(n < max_result_size))
				{
					result[n++] = (T*) cur_node;
				}
			}

			return n;
		}
	}

	Scene::Scene(){
		m_root_node = new SceneNode();

		m_camera.reset(
			new Camera(1.78f, 45.0f, 1.0f, 1000.0f,
			Vec3(0.0f, 0.0f, 5.0f),
			Vec3(0.0f, 0.0f, -1.0f),
			Vec3(0.0f, 1.0f, 0.0f)));
	}

	Scene::~Scene(){
		for (auto iter = m_sub_scene_list.begin();
			iter != m_sub_scene_list.end(); ++iter)
		{
			SubScenePtr s = iter->lock();
			if (s){
				s->set_root_node(nullptr);
			}
		}
		delete m_root_node;
	}

	void Scene::attach_sub_scene(const SubScenePtr& sub_scene){
		m_sub_scene_list.push_back(sub_scene);
		m_root_node->attach_child(sub_scene->root_node());
	}

	void Scene::detach_sub_scene(const SubScenePtr& sub_scene){
		m_sub_scene_list.remove_if([&](const WeakPtr<SubScene>::Type& e)
		{
			return e.lock() == sub_scene;
		});
		m_root_node->detach_child(sub_scene->root_node());
	}

	void Scene::update(){
		m_root_node->update();
	}

	void Scene::make_drawing_request(DrawingRequest& request){
		request.init();
		request.set_camera(m_camera);
		Frustum frustum = make_frustum(m_camera);
		cull_geometry(request, frustum);
		cull_light(request, frustum);

		/*request->clean_up();
		CameraNode* cameras[max_result_size];
		U32 camera_num =
		find_all_node(m_root_node, cameras,
		[&](CameraNode* camera_node)
		{
		return true;
		});
		auto camera = cameras[0]->camera();
		request->set_camera(camera);
		auto frustum = make_frustum(camera);
		cull_light_with_shadow(request, frustum);
		delete frustum;*/
	}

	void Scene::cull_geometry(
		DrawingRequest& request, const Frustum& frustum)
	{
		//TODO:
		SceneNode* visible_set[max_result_size];
		U32 visible_set_size =
			find_all_node(m_root_node,
			NM_GEOMETRY_NODE, visible_set,
			[&](SceneNode* geometry)->bool
		{
			/*auto bv = geometry->bounding_volume();
			return bv->in_frustum(frustum);*/

			return true;
		});

		for (U32 i = 0; i < visible_set_size; ++i){
			NodeGeometryResource* resource =
				dynamic_cast<NodeGeometryResource*>(visible_set[i]->node_resource());
			if (resource){
				if (resource->geometry()->is_opaque())
					request.attach_opaque_geometry(resource->geometry());
				else
					request.attach_transparent_geometry(resource->geometry());
			}
		}
	}

	void Scene::cull_light(
		DrawingRequest& request, const Frustum& frustum)
	{
		SceneNode* lights[max_result_size];
		U32 lights_size =
			find_all_node(m_root_node,
			NM_LIGHT_NODE, lights,
			[&](SceneNode* light)->bool
		{
			/*auto bv = light->bounding_volume();
			return (bv->in_frustum(frustum))
			&&(!light->has_shadow());*/

			return true;
		});

		for (U32 i = 0; i < lights_size; ++i){
			NodeLightResource* resource =
				dynamic_cast<NodeLightResource*>(
				lights[i]->node_resource());
			if (resource)
				request.attach_light(resource->light());
		};
	}

	Frustum Scene::make_frustum(CameraPtr camera){
		Mat4 matrix = camera->view_projection();
		Frustum frustum(matrix);
		return frustum;
	}
}