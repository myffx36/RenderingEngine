#pragma once

#include "GameObject.h"

namespace Necromancer{
	class DynamicGameObject :public GameObject{
	public:
		DynamicGameObject(const String&);
		virtual ~DynamicGameObject();
	private:
	};
	typedef SmartPtr<DynamicGameObject>::Type DynamicGameObjectPtr;
}

