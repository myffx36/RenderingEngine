#include "stdafx.h"
#include "PIL.h"

namespace Necromancer{
	PIL* PIL::s_instance = nullptr;

	PIL::PIL(Configuration* config){
		if(s_instance != nullptr){
			delete s_instance;
		}

		s_instance = this;
	}

	PIL::~PIL(){
	}

	PIL* PIL::get_instance(){
		return s_instance;
	}

	void PIL::set_instance(PIL* instance){
		s_instance = instance;
	}
}