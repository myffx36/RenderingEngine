#pragma once

#include "CommonUtility.h"
#include "GameObject.h"
#include "Container.hpp"

namespace Necromancer{
	class GameWorld;
	class AnimationEngine;
	class HID;
	class PhysicsEngine;
	class RenderingEngine;
	//class ScriptInterpreter;
	class Configuration;
	class PIL;
	class Timer;
	class ResourceManager;
	class Library;
	class Logger;

	class Necromancer{
		DEF_PTR_MEMBER_AND_GETTER(GameWorld, game_world);
		DEF_PTR_MEMBER_AND_GETTER(AnimationEngine, animation_engine);
		DEF_PTR_MEMBER_AND_GETTER(HID, hid);
		DEF_PTR_MEMBER_AND_GETTER(RenderingEngine, rendering_engine);
		DEF_PTR_MEMBER_AND_GETTER(PhysicsEngine, physics_engine);
		//DEF_PTR_MEMBER_AND_GETTER(ScriptInterpreter, script_interpreter);
		DEF_PTR_MEMBER_AND_GETTER(PIL, pil);
		DEF_PTR_MEMBER_AND_GETTER(ResourceManager, resource_manager);
		DEF_PTR_MEMBER_AND_GETTER(Logger, logger);
	public:
		Necromancer(Configuration*);
		~Necromancer();
		void main_loop();
		void end();
		void process_a_frame();

		static Necromancer* get_instance();
		
		U32 attach_game_object(const GameObjectPtr&);
		void detach_game_object(const GameObjectPtr&);
	private:
		void wait_some_time();
	private:
		bool m_is_end;
		Timer* m_timer;
		static Necromancer* s_instance;
	};
}

