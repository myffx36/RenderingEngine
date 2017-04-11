#include "stdafx.h"
#include "AnimationEngine.h"

namespace Necromancer{
	void AnimationEngine::update(F32 dlt_time){
		for (U32 i = 0; i < m_skeleton_instance_num; ++i){
			m_skeleton_instances[i]->update(dlt_time);
		}
	}

	void AnimationEngine::attach_skeleton_instance(
		const SkeletonInstancePtr& skeleton_instance)
	{
		m_skeleton_instances[m_skeleton_instance_num++] = 
			skeleton_instance;
	}

	AnimationEngine::AnimationEngine(Configuration*){
		m_skeleton_instance_num = 0;
		m_quit = false;
	}

	AnimationEngine::~AnimationEngine(){
	}

	void AnimationEngine::begin(){
		m_thread = std::move(std::thread(
			&Necromancer::AnimationEngine::thread_main, this));
	}

	void AnimationEngine::send_end_message(){
		m_quit = true;
	}

	void AnimationEngine::wait_for_end(){
		m_thread.join();
	}

	void AnimationEngine::thread_main(){
		while(!m_quit){
			wait_for_ready();
			F32 dlt_time = get_dlt_time();
			update(dlt_time);
			notify_for_finish();
		}
	}

	void AnimationEngine::wait_for_ready(){
	}

	F32 AnimationEngine::get_dlt_time(){
		return 0.0167f;
	}

	void AnimationEngine::notify_for_finish(){
		m_finish_promise.set_value(1);
	}

	std::promise<int>* AnimationEngine::make_finish_promise(
		std::promise<int>&& future)
	{
		m_finish_promise = std::move(std::promise<int>());
		m_finish_promise.set_value_at_thread_exit(1);
		return &m_finish_promise;
	}

	void AnimationEngine::init_for_other_thread(){
	}
}