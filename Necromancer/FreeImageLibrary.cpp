#include "stdafx.h"
#include "FreeImageLibrary.h"

extern "C"{
}

namespace Necromancer{
	void init_free_image(){
		FreeImage_Initialise();
	}

	void deinit_free_image(){
		FreeImage_DeInitialise();
	}
}