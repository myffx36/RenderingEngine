#pragma once

#include "StaticGameObject.h"

namespace Necromancer{
	class StaticLandScape :public StaticGameObject{
	public:
		StaticLandScape(const String& file_name);
		virtual ~StaticLandScape();
	private:
	};
	typedef SmartPtr<StaticLandScape>::Type StaticLandScapePtr;
}

