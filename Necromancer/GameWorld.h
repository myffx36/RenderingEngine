#pragma once

#include "Container.hpp"
#include "CommonUtility.h"
#include "GameObject.h"

namespace Necromancer{
	class GameWorld{
	public:
		virtual ~GameWorld();

		GameObjectPtr get_game_object_by_id(U32);
		GameObjectPtr get_game_object_by_name(const String&);

		U32 attach_game_object(const GameObjectPtr& game_object);

		void update_for_physics();

		virtual void update();
	private:
		void set_game_object(
			U32, const String&, const GameObjectPtr&);
	private:
		Map<U32, GameObjectPtr>::Type m_id_map;
		Map<String, GameObjectPtr>::Type m_name_map;
	};
}

