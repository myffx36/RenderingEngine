#include "stdafx.h"
#include "Necromancer.h"

#include "PIL.h"
#include "RenderingEngine.h"
#include "PhysicsEngine.h"
#include "AnimationEngine.h"
#include "GameWorld.h"
#include "HID.h"
//#include "ScriptInterpreter.h"
#include "Timer.h"
#include "Logger.h"

#include "Configuration.h"
#include "PILFactory.h"

namespace Necromancer{
	Necromancer* Necromancer::s_instance = nullptr;

	Necromancer::Necromancer(Configuration* config){
		PILFactory* pil_factory = new PILFactory();

		m_pil = pil_factory->create_pil(config);
		m_rendering_engine = new RenderingEngine(config, m_pil);
		m_animation_engine = new AnimationEngine(config);
		m_physics_engine = new PhysicsEngine(config);
		m_hid = new HID(config, m_pil);
		//m_script_interpreter = new ScriptInterpreter(config);
		m_game_world = new GameWorld();
		m_logger = new Logger();

		m_is_end = false;

		if(nullptr == s_instance){
			s_instance = this;
		}else{
			delete s_instance;
			s_instance = this;
		}

		m_timer = new Timer();
	}

	Necromancer::~Necromancer(){
		delete m_game_world;
		//delete m_script_interpreter;
		delete m_hid;
		delete m_physics_engine;
		delete m_animation_engine;
		delete m_rendering_engine;
		delete m_pil;
	}

	Necromancer* Necromancer::get_instance(){
		return s_instance;
	}

	void Necromancer::process_a_frame(){
		wait_some_time();
		F32 dlt_time = m_timer->get_dlt_time();
		m_hid->process_input_event();
		m_animation_engine->update(dlt_time);
		m_physics_engine->update(dlt_time);
		m_game_world->update();
		m_rendering_engine->draw_scene();
	}

	void Necromancer::main_loop(){
		m_hid->begin();
		m_rendering_engine->begin();
		//m_pil->begin();
		/*auto window = m_pil->pil_window();
		while(!m_is_end){
			if(window->handle_window_message()){
				process_a_frame();
			}
		}*/
		m_pil->wait_for_end();
		m_hid->wait_for_end();
		m_rendering_engine->wait_for_end();
	}

	void Necromancer::end(){
		m_hid->send_end_message();
		m_pil->send_end_message();
		m_rendering_engine->send_end_message();
		m_is_end = true;
	}

	void Necromancer::wait_some_time(){
		const U32 min_frame_time = 10;
		static U32 last_update_time = 0;
		if(last_update_time != 0){
			U32 current = m_timer->get_now();
			U32 dlt = current - last_update_time;
			while(dlt < min_frame_time){
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				current = m_timer->get_now();
				dlt = current - last_update_time;
			}
		}
		last_update_time = m_timer->get_now();
	}

	U32 Necromancer::attach_game_object(const GameObjectPtr& game_object){
		m_rendering_engine->attach_sub_scene(game_object->sub_scene());
		if(game_object->physics_object()){
			//m_physics_engine->attach_physics_object(
			//	game_object->physics_object());
		}
		return m_game_world->attach_game_object(game_object);
	}

	void Necromancer::detach_game_object(const GameObjectPtr& game_object){
		m_rendering_engine->detach_sub_scene(game_object->sub_scene());
		if (game_object->physics_object()){
			/*m_physics_engine->detach_physics_object(
				game_object->physics_object());*/
		}
	}
}