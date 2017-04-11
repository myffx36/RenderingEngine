#pragma once

#include "Container.hpp"
#include "Transform.h"

namespace Necromancer{
	class PhysicsEngine;
	class GameObject;
	class PhysicsObject{
	public:
		virtual ~PhysicsObject();

		virtual void detach_to_world(PhysicsEngine*) = 0;
		virtual void attach_to_world(PhysicsEngine*) = 0;

		virtual void update_game_object(GameObject*) = 0;
		virtual void set_transform(const Transform& transform) = 0;
	private:
	};

	typedef SmartPtr<PhysicsObject>::Type PhysicsObjectPtr;
}

