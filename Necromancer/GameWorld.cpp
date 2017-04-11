#include "stdafx.h"
#include "GameWorld.h"

namespace Necromancer{
	GameWorld::~GameWorld(){
	}

	void GameWorld::set_game_object(U32 id, 
		const String& name, const GameObjectPtr& object)
	{
		m_id_map[id] = object;
		m_name_map[name] = object;
	}

	GameObjectPtr GameWorld::get_game_object_by_id(U32 id){
		auto iter = m_id_map.find(id);
		if(iter == m_id_map.end()){
			return GameObjectPtr((GameObject*)nullptr);
		}else{
			return iter->second;
		}
	}

	GameObjectPtr GameWorld::get_game_object_by_name(const String& name){
		auto iter = m_name_map.find(name);
		if(iter == m_name_map.end()){
			return GameObjectPtr((GameObject*)nullptr);
		}else{
			return iter->second;
		}
	}

	void GameWorld::update_for_physics(){
		for(auto iter = m_id_map.begin();iter != m_id_map.end();++ iter){
			iter->second->update_physics();
		}
	}

	U32 GameWorld::attach_game_object(const GameObjectPtr& game_object){
		static U32 cur_id = 0;
		U32 uid = cur_id ++;
		set_game_object(uid, game_object->name(), game_object);

		return uid;
	}

	void GameWorld::update(){
		for (auto iter = m_id_map.begin(); iter != m_id_map.end(); ++iter){
			iter->second->update_physics();
		}
	}
}