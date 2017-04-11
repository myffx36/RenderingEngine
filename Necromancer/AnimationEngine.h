#pragma once

#include "CommonUtility.h"
#include "Container.hpp"
#include "SkeletonInstance.h"

namespace Necromancer{
	class Configuration;
	class AnimationEngine{
		DEF_DATA_MEMBER_AND_GETTER(U32, skeleton_instance_num);
	public:
		AnimationEngine(Configuration*);
		virtual ~AnimationEngine();

		void update(F32 dlt_time);

		void attach_skeleton_instance(const SkeletonInstancePtr&);
		void detach_skeleton_instance(const SkeletonInstancePtr&);
		void detach_skeleton_instance(const String& name);

		virtual void begin();
		virtual void send_end_message();
		virtual void wait_for_end();
		virtual void thread_main();

		void init_for_other_thread();

		std::promise<int>* make_finish_promise(std::promise<int>&& future);
	private:
		virtual void wait_for_ready();
		virtual F32 get_dlt_time();
		virtual void notify_for_finish();
	private:
		static const U32 ks_max_skeleton_instance_size = 128;
		Array<SkeletonInstancePtr, ks_max_skeleton_instance_size>::Type
			m_skeleton_instances;
		std::thread m_thread;
		bool m_quit;
		std::promise<int> m_finish_promise;
	};
}

