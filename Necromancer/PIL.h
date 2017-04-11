#pragma once

#include "CommonUtility.h"
#include "ThreadProcessor.h"

namespace Necromancer{
	class PILWindow;
	class PILHIDInfoGetter;
	class PILLLGI;
	class PILGSSupport;
	class PILShaderManager;
	class Configuration;
	class PIL{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(PILHIDInfoGetter, pil_hid_info_getter);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(PILLLGI, pil_llgi);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(PILShaderManager, pil_shader_manager);

		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, screen_width);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, screen_height);
	public:
		PIL(Configuration*);
		virtual ~PIL();

		static PIL* get_instance();
		static void set_instance(PIL*);

		virtual void wait_for_end() = 0;
		virtual void send_end_message() = 0;
	private:

		static PIL* s_instance;
	};
}

