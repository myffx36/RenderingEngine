#pragma once

namespace Necromancer{
	struct KeyboardEvent;
	class KeyboardHandler{
	public:
		KeyboardHandler();
		virtual ~KeyboardHandler();

		virtual void handle_request(KeyboardEvent*);

		void pass_to_next(KeyboardEvent*);

		void set_successor(KeyboardHandler*);
		KeyboardHandler* successor();
	private:
		virtual void do_handle_request(KeyboardEvent*);
	private:
		KeyboardHandler* m_successor;
	};
}

