#include "stdafx.h"
#include "ThreadProcessor.h"

#include "CommonUtility.h"

namespace Necromancer{
	ThreadProcessor::~ThreadProcessor(){
	}

	void ThreadProcessor::begin(){
		m_thread = std::move(std::thread(run_main, this));
	}

	void ThreadProcessor::run_main(ThreadProcessor* tp){
		tp->thread_main();
	}

	void ThreadProcessor::wait_for_end(){
		m_thread.join();
	}

	void ThreadProcessor::add_notify(std::promise<int>* promise){
		m_notifiy_queue.push_back(promise);
	}

	void ThreadProcessor::notify_all(){
		for(U32 i = 0;i < m_notifiy_queue.size();++ i){
			m_notifiy_queue[i]->set_value(0);
		}

		m_notifiy_queue.clear();
	}
}