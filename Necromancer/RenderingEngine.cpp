#include "stdafx.h"
#include "RenderingEngine.h"

#include "Configuration.h"
#include "PIL.h"
#include "PILLLGI.h"
#include "GraphicPipeline.h"
#include "Scene.h"
#include "ForwardGraphicPipeline.h"
#include "DeferredGraphicPipeline.h"
#include "DrawingRequest.h"
#include "NodeObserverManager.h"
#include "TwoDObject.h"

//temp
#include "Necromancer.h"
#include "AnimationEngine.h"


namespace Necromancer{
	RenderingEngine::RenderingEngine(Configuration* config,
		PIL* pil)
	{
		m_quit = false;
		PILLLGI* llgi = pil->pil_llgi();
		m_scene = new Scene();
		m_graphic_pipeline = new DeferredGraphicPipeline(config, llgi);
		//m_graphic_pipeline = new ForwardGraphicPipeline(config, llgi);
		m_camera = m_scene->camera();
		m_node_observer_manager = new NodeObserverManager();
	}

	RenderingEngine::~RenderingEngine(){

		std::for_each(m_two_d_object.begin(), m_two_d_object.end(), 
			[&](TwoDObject* object)
		{
			delete object;
		});

		delete m_node_observer_manager;
		delete m_scene;
		delete m_graphic_pipeline;
	}

	void RenderingEngine::attach_sub_scene(const SubScenePtr& sub_scene){
		m_scene->attach_sub_scene(sub_scene);
	}

	void RenderingEngine::detach_sub_scene(const SubScenePtr& sub_scene){
		m_scene->detach_sub_scene(sub_scene);
	}

	void RenderingEngine::draw_scene(){
		static DrawingRequest request;

		m_scene->update();
		m_scene->make_drawing_request(request);

		m_graphic_pipeline->process_drawing_request(&request);

		std::for_each(m_two_d_object.begin(), m_two_d_object.end(), 
			[&](TwoDObject* object)
		{
			object->draw();
		});
		
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->swap_buffer();
	}

	void RenderingEngine::attach_two_d_obejct(TwoDObject* object){
		m_two_d_object.push_back(object);
	}

	void RenderingEngine::detach_two_d_object(TwoDObject* object){
		auto iter = std::find(m_two_d_object.begin(),
			m_two_d_object.end(), object);

		if(iter != m_two_d_object.end()){
			m_two_d_object.erase(iter);
		}
	}

	void RenderingEngine::begin(){
		m_thread = std::move(std::thread(
			&::Necromancer::RenderingEngine::thread_main,
			this));
	}

	void RenderingEngine::wait_for_end(){
		m_thread.join();
	}

	void RenderingEngine::send_end_message(){
		m_quit = true;
	}

	void RenderingEngine::thread_main(){
		Necromancer* nec = Necromancer::get_instance();
		AnimationEngine* ae = nec->animation_engine();
		ae->init_for_other_thread();
		while(!m_quit){
			ae->update(0.016f);
			draw_scene();
		}
	}
}