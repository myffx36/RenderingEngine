#pragma once

#include "GameObject.h"

namespace Necromancer{
	class StaticGameObject :public GameObject{
	public:
		StaticGameObject(const String& name);
		virtual ~StaticGameObject();
	private:
	};
	typedef SmartPtr<StaticGameObject>::Type StaticGameObjectPtr;
}

