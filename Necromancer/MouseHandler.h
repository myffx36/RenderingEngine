#pragma once

namespace Necromancer{
	struct MouseEvent;
	class MouseHandler{
	public:
		MouseHandler();
		virtual ~MouseHandler();

		virtual void handle_request(MouseEvent* event);
		
		void pass_to_next(MouseEvent*);

		void set_successor(MouseHandler*);
		MouseHandler* successor();
	private:
		virtual void do_handle_request(MouseEvent*);
	private:
		MouseHandler* m_successor;
	};
}

