#pragma once

#include "Container.hpp"
#include "CommonUtility.h"
#include "SubScene.h"

#include "PhysicsObject.h"

namespace Necromancer{
	class Property;
	class GameObject{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, id);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, name);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(SubScenePtr, sub_scene);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(PhysicsObjectPtr, physics_object);
	public:
		GameObject(const String&);

		Property* get_property(const String&);
		void set_porperty(const String&, Property*);

		virtual ~GameObject();

		void play_animation(const String& name);

		template<typename T>
		T* get_typed_property(const String& name){
			return dynamic_cast<T*>(get_property(name));
		}

		virtual void update_physics();

		void set_transform(const Transform&);
		Transform transform() const;
		void set_sub_scene_transform(const Transform&);
	private:
		Map<String, Property*>::Type m_property_map;
	};

	typedef SmartPtr<GameObject>::Type GameObjectPtr;
}

