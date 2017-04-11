#include "stdafx.h"
#include "PILFactory.h"

#include "Configuration.h"
#include "PIL.h"
#include "D3D11_1PIL.h"

namespace Necromancer{
	PILFactory::~PILFactory(){
	}

	PIL* PILFactory::create_pil(Configuration* config){
		String platform = config->get_value("Platform", "platform");
		if(platform == "D11_1"){
			return new D3D11_1PIL(config);
		}

		return nullptr;
	}
}