#pragma once

#include "Container.hpp"

namespace Necromancer{
	class ThreadProcessor{
	public:
		virtual ~ThreadProcessor();

		virtual void begin();
		virtual void wait_for_end();
		
		virtual void send_end_notify() = 0;
		virtual void thread_main() = 0;

		virtual void add_notify(std::promise<int>* promise);
		virtual void notify_all();
	private:
		static void run_main(ThreadProcessor* tp);
	protected:
		std::thread m_thread;

		Vector<std::promise<int>*>::Type m_notifiy_queue;
	};
}

