#pragma once

#include "GameObject.h"

namespace Necromancer{
	class NECMotionState:public btMotionState{
	public:

		NECMotionState(const GameObjectPtr&);

		virtual ~NECMotionState();

		virtual void	getWorldTransform(btTransform& worldTrans) const;

		//Bullet only calls the update of worldtransform for active objects
		virtual void	setWorldTransform(const btTransform& worldTrans);
	private:
		GameObjectPtr m_game_object;
	};
}

