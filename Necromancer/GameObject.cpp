#include "stdafx.h"
#include "GameObject.h"

#include "Necromancer.h"
#include "Property.h"
//#include "AnimationEngine.h"
#include "GameWorld.h"

//#include "PhysicsObject.h"

namespace Necromancer{
	GameObject::GameObject(const String& name){
	}

	GameObject::~GameObject(){
		for (auto iter = m_property_map.begin();
			iter != m_property_map.end(); ++iter)
		{
			if (iter->second != nullptr){
				delete iter->second;
			}
		}
	}

	Property* GameObject::get_property(const String& name){
		auto iter = m_property_map.find(name);
		if (iter == m_property_map.end()){
			return nullptr;
		}
		else{
			return iter->second;
		}
	}

	void GameObject::set_porperty(const String& name,
		Property* property)
	{
		m_property_map[name] = property;
	}

	void GameObject::play_animation(const String& name){
		//AnimationPtr animation =
		//	m_sub_scene->get_animation(name);
		//Necromancer* necromancer = Necromancer::get_instance();
		//AnimationEngine* animation_engine = necromancer->anmation_engine();
		//animation_engine->play_animation(m_sub_scene, animation);
	}

	void GameObject::update_physics(){
		if (m_physics_object != nullptr){
			m_physics_object->update_game_object(this);
		}
	}

	void GameObject::set_transform(const Transform& transform){
		//m_sub_scene->set_transform(transform);
		//if (m_physics_object)
		//	m_physics_object->set_transform(transform);
	}

	Transform GameObject::transform() const{
		return m_sub_scene->transform();
	}

	void GameObject::set_sub_scene_transform(const Transform& transform){
		m_sub_scene->set_transform(transform);
	}
}