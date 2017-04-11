#pragma once

#include "CommonUtility.h"

namespace Necromancer{
	class Configuration;
	class PhysicsEngine{
	public:
		PhysicsEngine(Configuration*);
		virtual ~PhysicsEngine();

		virtual void update(F32 dlt);
	private:
	};
}

