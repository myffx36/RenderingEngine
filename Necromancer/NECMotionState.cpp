#include "stdafx.h"
#include "NECMotionState.h"

namespace Necromancer{
	NECMotionState::NECMotionState(const GameObjectPtr& game_object)
		:m_game_object(game_object)
	{

	}

	NECMotionState::~NECMotionState(){

	}

	void NECMotionState::setWorldTransform(const btTransform& worldTrans){
		btVector3 bt_origin = worldTrans.getOrigin();
		btQuaternion bt_quaternion = worldTrans.getRotation();
		Translation translation(Vec3(bt_origin.x(), 
			bt_origin.y(), bt_origin.z()));
		Rotation rotation(Quaternion(bt_quaternion.x(), 
			bt_quaternion.y(), bt_quaternion.z(), bt_quaternion.w()));
		Transform transform;
		transform.set_translation(translation);
		transform.set_rotation(rotation);
		m_game_object->set_transform(transform);
	}

	void NECMotionState::getWorldTransform(btTransform& worldTrans) const{
		btVector3 bt_origin(
			m_game_object->transform().translation().vec().x,
			m_game_object->transform().translation().vec().y,
			m_game_object->transform().translation().vec().z);
		btQuaternion bt_rotation(
			m_game_object->transform().rotation().quaternion()[0],
			m_game_object->transform().rotation().quaternion()[1],
			m_game_object->transform().rotation().quaternion()[2],
			m_game_object->transform().rotation().quaternion()[3]);
		worldTrans.setOrigin(bt_origin);
		worldTrans.setRotation(bt_rotation);
	}
}