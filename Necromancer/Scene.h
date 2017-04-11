#pragma once

#include "CommonUtility.h"
#include "DrawingRequest.h"
#include "Camera.h"
#include "SubScene.h"

namespace Necromancer{
	class SceneNode;
	class Frustum;
	class Scene{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(SceneNode, root_node);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(CameraPtr, camera);
	public:
		Scene();
		~Scene();

		void attach_sub_scene(const SubScenePtr&);
		void detach_sub_scene(const SubScenePtr&);

		void make_drawing_request(DrawingRequest&);
		void update();
	private:
		Frustum make_frustum(CameraPtr camera);
		void cull_geometry(DrawingRequest&, const Frustum&);
		void cull_light(DrawingRequest&, const Frustum&);
		/*void cull_light_with_shadow(DrawingRequest*, Frustum*);
		void cull_shadow_geometry(GeometryNode**, U32& num, Light*);*/
	private:
		List<WeakPtr<SubScene>::Type>::Type m_sub_scene_list;
	};
}

