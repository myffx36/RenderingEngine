#include "stdafx.h"
#include "Necromancer.h"

#include "Configuration.h"

//#include "ShaderFragmentInit.h"
#include "Scene.h"
#include "RenderingEngine.h"
#include "Camera.h"
#include "HID.h"
#include "FBXFileUtility.h"
#include "SceneNode.h"
#include "NodeLightResource.h"

Necromancer::SubScenePtr create_light() {
	using namespace Necromancer;
	SceneNode* node = new SceneNode();
	Light light;
	light.set_color(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	light.set_light_type(NM_LIGHT_POINT);
	light.set_params(Vec4(1.0, 0.007, 0.0002, 600.0f));
	NodeLightResource* light_resource = new NodeLightResource(light);
	node->set_node_resource(light_resource);
	SubScenePtr sub_scene(new SubScene());
	sub_scene->set_root_node(node);
	Transform transform;
	transform.set_translation(0.0f, 0.0f, 5.0f);
	node->set_local_transform(transform);
	return sub_scene;
}

int main()
{
	Necromancer::Configuration config;
	config.load_file("config.ini");
	Necromancer::Necromancer* nec = new ::Necromancer::Necromancer(&config);
	//init_shader_fragments();
	//nec->hid()->attach_keyboard_handler(5, tkh);

	std::string file_name("F:\\tmp\\dragon-fbx\\dragon.FBX");
	//std::cout << "file_name:" << std::endl;
	//std::cin >> file_name;

	auto sub_scene =
		Necromancer::create_sub_scene_from_fbx_file(file_name.c_str());
	nec->rendering_engine()->attach_sub_scene(sub_scene);
	sub_scene = create_light();
	nec->rendering_engine()->attach_sub_scene(sub_scene);

	nec->main_loop();
	delete nec;

	return 0;
}