#include "stdafx.h"
#include "WrapModules.h"

#include "BasicMath.h"
#include "PIL.h"
#include "PILLLGI.h"
#include "RenderingResource.h"
#include "Necromancer.h"
#include "Scene.h"
#include "SceneNode.h"
#include "NodeResource.h"
#include "PILHIDInfoGetter.h"
#include "HID.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "Property.h"
#include "IntegerProperty.h"
#include "StringProperty.h"
#include "NECMesh.h"
#include "ResourceManager.h"

using namespace boost::python;
using namespace Necromancer;

namespace{
	//Some helper functions for math module;
	float Vec2__getitem__(const Vec2& vec2, U32 p){
		if(p < 0){
			p = 2 + p;
		}
		if(p > 1){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		return vec2[p];
	}

	void Vec2__setitem__(Vec2& vec2, U32 p, float val){
		if(p < 0){
			p = 2 + p;
		}
		if(p > 1){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		vec2[p] = val;
	}

	float Vec3__getitem__(const Vec3& v, U32 p){
		if(p < 0){
			p = 3 + p;
		}
		if(p > 2){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		return v[p];
	}

	void Vec3__setitem__(Vec3& v, U32 p, float val){
		if(p < 0){
			p = 3 + p;
		}
		if(p > 2){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		v[p] = val;
	}

	float Vec4__getitem__(const Vec4& v, U32 p){
		if(p < 0){
			p = 4 + p;
		}
		if(p > 3){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		return v[p];
	}

	void Vec4__setitem__(Vec4& v, U32 p, float val){
		if(p < 0){
			p = 4 + p;
		}
		if(p > 3){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		v[p] = val;
	}

	Vec3& Mat3__getitem__(Mat3& m, U32 p){
		if(p < 0){
			p = 3 + p;
		}
		if(p > 2){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		return m[p];
	}

	void Mat3__setitem__(Mat3& m, U32 p, const Vec3& val){
		if(p < 0){
			p = 3 + p;
		}
		if(p > 2){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		m[p] = val ;
	}

	Vec4& Mat4__getitem__(Mat4& m, U32 p){
		if(p < 0){
			p = 4 + p;
		}
		if(p > 3){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		return m[p];
	}

	void Mat4__setitem__(Mat4& m, U32 p, const Vec4& val){
		if(p < 0){
			p = 4 + p;
		}
		if(p > 3){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		m[p] = val;
	}

	float Quat__getitem__(const Quaternion& q, U32 p){
		if(p < 0){
			p = 4 + p;
		}
		if(p > 3){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		return q[p];
	}

	void Quat__setitem__(Quaternion& q, U32 p, float val){
		if(p < 0){
			p = 4 + p;
		}
		if(p > 3){
			PyErr_SetString( PyExc_IndexError, "index out of range" );
			boost::python::throw_error_already_set(); 
		}
		q[p] = val;
	}

	const F32 (*vec2_dot_fun)(const Vec2&, const Vec2&) = &::Necromancer::dot;
	const F32 (*vec2_length_fun)(const Vec2&) = &::Necromancer::length;

	const F32 (*vec3_dot_fun)(const Vec3&, const Vec3&) = &::Necromancer::dot;
	const F32 (*vec3_length_fun)(const Vec3&) = &::Necromancer::length;
	const Vec3 (*vec3_normalize_fun)(const Vec3&) = &::Necromancer::normalize;

	const F32 (*vec4_dot_fun)(const Vec4&, const Vec4&) = &::Necromancer::dot;
	const F32 (*vec4_length_fun)(const Vec4&) = &::Necromancer::length;
	const Vec4 (*vec4_normalize_fun)(const Vec4&) = &::Necromancer::normalize;


	Mat4 (*translate_matrix_1)(F32, F32, F32) = &::Necromancer::translate_matrix;
	Mat4 (*translate_matrix_2)(const Vec3&) = &::Necromancer::translate_matrix;
	Mat4 (*rotate_matrix_1)(F32, F32, F32, F32) = &::Necromancer::rotate_matrix;
	Mat4 (*rotate_matrix_2)(const Vec3&, F32) = &::Necromancer::rotate_matrix;
	Mat4 (*scale_matrix_1)(F32, F32, F32) = &::Necromancer::scale_matrix;
	Mat4 (*scale_matrix_2)(const Vec3&) = &::Necromancer::scale_matrix;
	//end of help functions for math


	//helper functions for llgi
	PILLLGI* get_llgi(){
		::Necromancer::Necromancer* necromancer = 
			::Necromancer::Necromancer::get_instance();
		PIL* pil = necromancer->pil();

		return pil->pil_llgi();
	}
	//end of help functions of llgi

	//helper class and functions for scene graph
	class NodeResourceWrapper:public NodeResource, 
		public wrapper<NodeResource>
	{
	public:
		virtual void before_update(SceneNode* node){
			if(override fun = this->get_override("before_update")){
				fun(node);
			}else{
				this->before_update(node);
			}
		}

		virtual void update(SceneNode* node){
			if(override fun = this->get_override("update")){
				fun(node);
			}else{
				this->update(node);
			}
		}

		virtual void after_update(SceneNode* node){
			if(override fun = this->get_override("after_update")){
				fun(node);
			}else{
				this->after_update(node);
			}
		}

		void default_before_update(SceneNode* node){
			this->before_update(node);
		}

		void default_update(SceneNode* node){
			this->update(node);
		}

		void default_after_update(SceneNode* node){
			this->after_update(node);
		}

	private:
	};

	void (Transform::*transform_set_translation)(F32, F32, F32) = &Transform::set_translation;
	void (Transform::*transform_set_rotation)(F32, F32, F32, F32) = &Transform::set_rotation;
	void (Transform::*transform_set_scale)(F32, F32, F32) = &Transform::set_scale;
	//end of scene graph

	//help functions and wrapper for hid
	//end hid

	//help functions and wrapper for game play
	GameWorld* get_game_world(){
		::Necromancer::Necromancer* necromancer = 
			::Necromancer::Necromancer::get_instance();
		return necromancer->game_world();
	}

	class GameWorldWrapper:public GameWorld, public wrapper<GameWorld>{
	};

	class GameObjectWrapper:public GameObject, public wrapper<GameObject>{
	public:
		GameObjectWrapper(U32 id, String name)
			:GameObject(id, name)
		{
		}

		virtual ~GameObjectWrapper(){}
	};

	class PropertyWrapper:public Property, public wrapper<Property>{
	};
	//end game play

	//help functions and wrappers for game resource
	GameObjectPtr load_game_object_from_resource_file(U32 uid){
		::Necromancer::Necromancer* necromancer = 
			::Necromancer::Necromancer::get_instance();
		::Necromancer::ResourceManager* resource_manager = 
			necromancer->resource_manager();
		return resource_manager->load_game_resource<GameObject>(uid, false);
	}

	GameObjectPtr create_game_object_instance_from_resource_file(U32 uid){
		::Necromancer::Necromancer* necromancer = 
			::Necromancer::Necromancer::get_instance();
		::Necromancer::ResourceManager* resource_manager = 
			necromancer->resource_manager();
		return resource_manager->load_game_resource<GameObject>(uid, true);
	}

	void attach_resource_file(const char* file_name){
		::Necromancer::Necromancer* necromancer = 
			::Necromancer::Necromancer::get_instance();
		::Necromancer::ResourceManager* resource_manager = 
			necromancer->resource_manager();
		resource_manager->attach_file(file_name);
	}

	void detach_resource_file(const char* file_name){
		::Necromancer::Necromancer* necromancer = 
			::Necromancer::Necromancer::get_instance();
		::Necromancer::ResourceManager* resource_manager = 
			necromancer->resource_manager();
		resource_manager->detach_file(file_name);
	}
	//end game resource

	//help functions and wrapper for necromancer
	void end(){
		::Necromancer::Necromancer* necromancer = 
			::Necromancer::Necromancer::get_instance();

		necromancer->end();
	}

	U32 attach_game_object(GameObjectPtr go){
		::Necromancer::Necromancer* necromancer = 
			::Necromancer::Necromancer::get_instance();

		return necromancer->attach_game_object(go);
	}
	//end necromancer
}

BOOST_PYTHON_MODULE(Necromancer){
	//Math module.............
	class_<Vec2>("Vec2", init<>())
		.def(init<float, float>())
		.def_readwrite("x", &Vec2::x)
		.def_readwrite("y", &Vec2::y)
		.def(self + self)
		.def(self - self)
		.def(self * self)
		.def(self / self)
		.def(self * float())
		.def(self / float())
		.def(float() * self)
		.def("__getitem__", &Vec2__getitem__)
		.def("__setitem__", &Vec2__setitem__);
	def("dot", vec2_dot_fun);
	def("length", vec2_length_fun);

	class_<Vec3>("Vec3", init<>())
		.def(init<float, float, float>())
		.def(init<const Vec2&, float>())
		.def(init<float, const Vec2&>())
		.def_readwrite("x", &Vec3::x)
		.def_readwrite("y", &Vec3::y)
		.def_readwrite("z", &Vec3::z)
		.def(self + self)
		.def(self - self)
		.def(self * self)
		.def(self / self)
		.def(self * float())
		.def(self / float())
		.def(float() * self)
		.def("__getitem__", &Vec3__getitem__)
		.def("__setitem__", &Vec3__setitem__);
	def("dot", vec3_dot_fun);
	def("length", vec3_length_fun);
	def("cross", &::Necromancer::cross);
	def("normalize", vec3_normalize_fun);

	class_<Vec4>("Vec4", init<>())
		.def(init<float, float, float, float>())
		.def(init<const Vec3&, float>())
		.def(init<float, const Vec3&>())
		.def(init<const Vec2&, const Vec2&>())
		.def_readwrite("x", &Vec4::x)
		.def_readwrite("y", &Vec4::y)
		.def_readwrite("z", &Vec4::z)
		.def_readwrite("w", &Vec4::w)
		.def(self + self)
		.def(self - self)
		.def(self * self)
		.def(self / self)
		.def(self * float())
		.def(self / float())
		.def(float() * self)
		.def("__getitem__", &Vec4__getitem__)
		.def("__setitem__", &Vec4__setitem__);
	def("dot", vec4_dot_fun);
	def("length", vec4_length_fun);
	def("normalize", vec4_normalize_fun);

	class_<Mat3>("Mat3", init<>())
		.def(init<const Vec3&, const Vec3&, const Vec3&>())
		.def(init<float, float, float,
		float, float, float, float, float, float>())
		.def(self + self)
		.def(self - self)
		.def(self * self)
		.def(self * float())
		.def(self / float())
		.def(Vec3() * self)
		.def("__getitem__", &Mat3__getitem__, return_value_policy<copy_non_const_reference>())
		.def("__setitem__", &Mat3__setitem__);

	class_<Mat4>("Mat4", init<>())
		.def(init<const Vec4&, const Vec4&, const Vec4&, const Vec4&>())
		.def(self + self)
		.def(self - self)
		.def(self * self)
		.def(self * float())
		.def(self / float())
		.def(Vec4() * self)
		.def("__getitem__", &Mat4__getitem__, return_value_policy<copy_non_const_reference>())
		.def("__setitem__", &Mat4__setitem__);

	class_<Quaternion>("Quaternioin", init<>())
		.def(init<const Vec4&>())
		.def(init<float, float, float, float>())
		.def(self * self)
		.def("rotate_vec4", &Quaternion::rotate_vec4)
		.def("rotate_vec3", &Quaternion::rotate_vec3)
		.def("convert_to_matrix3x3", &Quaternion::convert_to_matrix3x3)
		.def("convert_to_matrix4x4", &Quaternion::convert_to_matrix4x4)
		.def("length", &Quaternion::length)
		.def("__getitem__", &Quat__getitem__)
		.def("__setitem__", &Quat__setitem__)
		.def("get_axis", &Quaternion::get_axis)
		.def("get_theta", &Quaternion::get_theta);

	def("inverse_quaternion", &inverse_quaternion);
	def("mat3_to_quaternion", &mat3_to_quaternion);
	def("mat4_to_quaternion", &mat4_to_quaternion);
	def("make_quanternion", &make_quanternion);
	def("make_quanternion_from_euler", &make_quanternion_from_euler);
	def("blend_lerp", &blend_lerp);
	def("blend_slerp", &blend_slerp);
	def("translate_matrix", translate_matrix_1);
	def("translate_matrix", translate_matrix_2);
	def("rotate_matrix", rotate_matrix_1);
	def("rotate_matrix", rotate_matrix_2);
	def("rotate_matrix_vec_to_vec", &rotate_matrix_vec_to_vec);
	def("scale_matrix", scale_matrix_1);
	def("scale_matrix", scale_matrix_2);
	def("perspective_projection_matrix", &perspective_projection_matrix);
	def("orthographic_projection_matrix", &orthographic_projection_matrix);
	def("look_at_matrix", &look_at_matrix);
	def("look_at_matrix_by_look_at_dir", &look_at_matrix_by_look_at_dir);
	def("inverse_matrix", &inverse_matrix);
	//Math module end

	//Rendering resource module begin...........  TODO add some more property
	class_<RenderingResource, RenderingResourcePtr>("RenderingResource");
	class_<BufferObject, BOPtr>("BufferObject");
	class_<VertexBuffer, VBPtr>("VertexBuffer");
	class_<IndexBuffer, IBPtr>("IndexBuffer");
	class_<ConstantBuffer, CBPtr>("ConstantBuffer");
	class_<VertexShader, VSPtr>("VertexShader");
	class_<FragmentShader, FSPtr>("FragmentShader");
	class_<Texture, TexPtr>("Texture");
	class_<Texture2D, Tex2DPtr>("Texture2D");
	class_<TextureCube, TexCubePtr>("TextureCube");
	class_<RenderTarget, RTPtr>("RenderTarget");
	class_<DepthStencil, DSPtr>("DepthStencil");
	class_<ReadableDepthStencil, RDSPtr>("ReadableDepthStencil");
	class_<RenderableTexture2D, RTex2DPtr>("RenderableTexture2D");
	class_<RenderableTextureCube, RTexCubePtr>("RenderableTextureCube");
	class_<GraphicState, GraphicStatePtr>("GraphicState");

	enum_<GeometryType>("GeometryType")
		.value("POINTS", NECROMANCER_GEOMETRY_POINTS)
		.value("LINES", NECROMANCER_GEOMETRY_LINES)
		.value("MESH", NECROMANCER_GEOMETRY_MESH);
	enum_<BindFlag>("BindFlag")
		.value("VS", BIND_TO_VS)
		.value("FS", BIND_TO_FS)
		.value("GS", BIND_TO_GS)
		.value("CS", BIND_TO_CS)
		.value("DS", BIND_TO_DS)
		.value("HS", BIND_TO_HS);
	//Rendering resource end.........

	//llgi modeule begin.....  TODO:add some more function
	//TODO : deal with the virtual class of LLGI
	/*def("get_llgi", get_llgi, 
	return_value_policy<reference_existing_object>());
	class_<PILLLGI>("LLGI")
	.def("enable_vertex_buffer", &PILLLGI::enable_vertex_buffer)
	.def("enable_index_buffer", &PILLLGI::enable_index_buffer)
	.def("enable_constant_buffer", &PILLLGI::enable_constant_buffer)
	.def("disable_vertex_buffer", &PILLLGI::disable_vertex_buffer)
	.def("disable_index_buffer", &PILLLGI::disable_index_buffer)
	.def("disable_constant_buffer", &PILLLGI::disable_constant_buffer)
	.def("enable_vertex_shader", &PILLLGI::enable_vertex_shader)
	.def("enable_fragment_shader", &PILLLGI::enable_fragment_shader)
	.def("disable_vertex_shader", &PILLLGI::disable_vertex_shader)
	.def("disable_fragment_shader", &PILLLGI::disable_fragment_shader)
	.def("enable_texture", &PILLLGI::enable_texture)
	.def("disable_texture", &PILLLGI::disable_texture);*/
	//end llgi................


	//Scene graph module begin.................... TODO: add some wrapper to support extend
	//TODO deal with virtual class of NodeResource
	//class_<NodeResource>("NodeResource");
	/*class_<NodeResourceWrapper, boost::noncopyable>("NodeResourceWrapper")
	.def("before_update", &NodeResource::before_update, &NodeResourceWrapper::default_before_update)
	.def("update", &NodeResource::update, &NodeResourceWrapper::default_update)
	.def("after_update", &NodeResource::after_update, &NodeResourceWrapper::default_after_update);*/
	class_<Transform>("Transform")
		.def("set_translation", transform_set_translation)
		.def("set_rotation", transform_set_rotation)
		.def("set_scale", transform_set_scale);
	class_<SceneNode>("SceneNode");
	class_<Scene>("Scene");
	//end scene graph


	//HID module begin...................... TODO:add some wrapper to support extend
	enum_<KeyboardEventType>("KeyboardEventType")
		.value("KEY_DOWN", KEY_DOWN)
		.value("KEY_UP", KEY_UP);
	enum_<KeyInfo>("KeyInfo")
		.value("NMK_0" ,			NMK_0)	
		.value("NMK_1" ,			NMK_1)
		.value("NMK_2" ,			NMK_2)
		.value("NMK_3" ,			NMK_3)
		.value("NMK_4" ,			NMK_4)
		.value("NMK_5" ,			NMK_5)
		.value("NMK_6" ,			NMK_6)
		.value("NMK_7" ,			NMK_7)
		.value("NMK_8" ,			NMK_8)
		.value("NMK_9" ,			NMK_9)
		.value("NMK_A" ,			NMK_A)
		.value("NMK_ABNT_C1" ,		NMK_ABNT_C1)
		.value("NMK_ABNT_C2" ,		NMK_ABNT_C2)
		.value("NMK_ADD" ,			NMK_ADD)
		.value("NMK_APOSTROPHE" ,	NMK_APOSTROPHE)
		.value("NMK_APPS" ,			NMK_APPS)
		.value("NMK_AT" ,			NMK_AT)
		.value("NMK_AX" ,			NMK_AX)
		.value("NMK_B" ,			NMK_B)
		.value("NMK_BACK" ,			NMK_BACK)
		.value("NMK_BACKSLASH" ,	NMK_BACKSLASH)
		.value("NMK_C" ,			NMK_C)
		.value("NMK_CALCULATOR" ,	NMK_CALCULATOR)
		.value("NMK_CAPITAL" ,		NMK_CAPITAL)
		.value("NMK_COLON" ,		NMK_COLON)
		.value("NMK_COMMA" ,		NMK_COMMA)
		.value("NMK_CONVERT" ,		NMK_CONVERT)
		.value("NMK_D" ,			NMK_D)
		.value("NMK_DECIMAL" ,		NMK_DECIMAL)
		.value("NMK_DELETE" ,		NMK_DELETE)
		.value("NMK_DIVIDE" ,		NMK_DIVIDE)
		.value("NMK_DOWN" ,			NMK_DOWN)
		.value("NMK_E" ,			NMK_E)
		.value("NMK_END" ,			NMK_END)
		.value("NMK_EQUALS" ,		NMK_EQUALS)
		.value("NMK_ESCAPE" ,		NMK_ESCAPE)
		.value("NMK_F" ,			NMK_F)
		.value("NMK_F1" ,			NMK_F1)
		.value("NMK_F2" ,			NMK_F2)
		.value("NMK_F3" ,			NMK_F3)
		.value("NMK_F4" ,			NMK_F4)
		.value("NMK_F5" ,			NMK_F5)
		.value("NMK_F6" ,			NMK_F6)
		.value("NMK_F7" ,			NMK_F7)
		.value("NMK_F8" ,			NMK_F8)
		.value("NMK_F9" ,			NMK_F9)
		.value("NMK_F10" ,			NMK_F10)
		.value("NMK_F11" ,			NMK_F11)
		.value("NMK_F12" ,			NMK_F12)
		.value("NMK_F13" ,			NMK_F13)
		.value("NMK_F14" ,			NMK_F14)
		.value("NMK_F15" ,			NMK_F15)
		.value("NMK_G" ,			NMK_G)
		.value("NMK_GRAVE" ,		NMK_GRAVE)
		.value("NMK_H" ,			NMK_H)
		.value("NMK_HOME" ,			NMK_HOME)
		.value("NMK_I" ,			NMK_I)
		.value("NMK_INSERT" ,		NMK_INSERT)
		.value("NMK_J" ,			NMK_J)
		.value("NMK_K" ,			NMK_K)
		.value("NMK_KANA" ,			NMK_KANA)
		.value("NMK_KANJI" ,		NMK_KANJI)
		.value("NMK_L" ,			NMK_L)
		.value("NMK_LBRACKET" ,		NMK_LBRACKET)
		.value("NMK_LCONTROL" ,		NMK_LCONTROL)
		.value("NMK_LEFT" ,			NMK_LEFT)
		.value("NMK_LMENU" ,		NMK_LMENU)
		.value("NMK_LSHIFT" ,		NMK_LSHIFT)
		.value("NMK_LWIN" ,			NMK_LWIN)
		.value("NMK_M" ,			NMK_M)
		.value("NMK_MAIL" ,			NMK_MAIL)
		.value("NMK_MEDIASELECT" ,	NMK_MEDIASELECT)
		.value("NMK_MEDIASTOP" ,	NMK_MEDIASTOP)
		.value("NMK_MINUS" ,		NMK_MINUS)
		.value("NMK_MULTIPLY" ,		NMK_MULTIPLY)
		.value("NMK_MUTE" ,			NMK_MUTE)
		.value("NMK_MYCOMPUTER" ,	NMK_MYCOMPUTER)
		.value("NMK_N" ,			NMK_N)
		.value("NMK_NEXT" ,			NMK_NEXT)
		.value("NMK_NEXTTRACK" ,	NMK_NEXTTRACK)
		.value("NMK_NOCONVERT" ,	NMK_NOCONVERT)
		.value("NMK_NUMLOCK" ,		NMK_NUMLOCK)
		.value("NMK_NUMPAD0" ,		NMK_NUMPAD0)
		.value("NMK_NUMPAD1" ,		NMK_NUMPAD1)
		.value("NMK_NUMPAD2" ,		NMK_NUMPAD2)
		.value("NMK_NUMPAD3" ,		NMK_NUMPAD3)
		.value("NMK_NUMPAD4" ,		NMK_NUMPAD4)
		.value("NMK_NUMPAD5" ,		NMK_NUMPAD5)
		.value("NMK_NUMPAD6" ,		NMK_NUMPAD6)
		.value("NMK_NUMPAD7" ,		NMK_NUMPAD7)
		.value("NMK_NUMPAD8" ,		NMK_NUMPAD8)
		.value("NMK_NUMPAD9" ,		NMK_NUMPAD9)
		.value("NMK_NUMPADCOMMA" ,	NMK_NUMPADCOMMA)
		.value("NMK_NUMPADENTER" ,	NMK_NUMPADENTER)
		.value("NMK_NUMPADEQUALS" ,	NMK_NUMPADEQUALS)
		.value("NMK_O" ,			NMK_O)
		.value("NMK_OEM_102" ,		NMK_OEM_102)
		.value("NMK_P" ,			NMK_P)
		.value("NMK_PAUSE" ,		NMK_PAUSE)
		.value("NMK_PERIOD" ,		NMK_PERIOD)
		.value("NMK_PLAYPAUSE" ,	NMK_PLAYPAUSE)
		.value("NMK_POWER" ,		NMK_POWER)
		.value("NMK_PREVTRACK" ,	NMK_PREVTRACK)
		.value("NMK_PRIOR" ,		NMK_PRIOR)
		.value("NMK_Q" ,			NMK_Q)
		.value("NMK_R" ,			NMK_R)
		.value("NMK_RBRACKET" ,		NMK_RBRACKET)
		.value("NMK_RCONTROL" ,		NMK_RCONTROL)
		.value("NMK_RETURN" ,		NMK_RETURN)
		.value("NMK_RIGHT" ,		NMK_RIGHT)
		.value("NMK_RMENU" ,		NMK_RMENU)
		.value("NMK_RSHIFT" ,		NMK_RSHIFT)
		.value("NMK_RWIN" ,			NMK_RWIN)
		.value("NMK_S" ,			NMK_S)
		.value("NMK_SCROLL" ,		NMK_SCROLL)
		.value("NMK_SEMICOLON" ,	NMK_SEMICOLON)
		.value("NMK_SLASH" ,		NMK_SLASH)
		.value("NMK_SLEEP" ,		NMK_SLEEP)
		.value("NMK_SPACE" ,		NMK_SPACE)
		.value("NMK_STOP" ,			NMK_STOP)
		.value("NMK_SUBTRACT" ,		NMK_SUBTRACT)
		.value("NMK_SYSRQ" ,		NMK_SYSRQ)
		.value("NMK_T" ,			NMK_T)
		.value("NMK_TAB" ,			NMK_TAB)
		.value("NMK_U" ,			NMK_U)
		.value("NMK_UNDERLINE" ,	NMK_UNDERLINE)
		.value("NMK_UNLABELED" ,	NMK_UNLABELED)
		.value("NMK_UP" ,			NMK_UP)
		.value("NMK_V" ,			NMK_V)
		.value("NMK_VOLUMEDOWN" ,	NMK_VOLUMEDOWN)
		.value("NMK_VOLUMEUP" ,		NMK_VOLUMEUP)
		.value("NMK_W" ,			NMK_W)
		.value("NMK_WAKE" ,			NMK_WAKE)
		.value("NMK_WEBBACK" ,		NMK_WEBBACK)
		.value("NMK_WEBFAVORITES" ,	NMK_WEBFAVORITES)
		.value("NMK_WEBFORWARD" ,	NMK_WEBFORWARD)
		.value("NMK_WEBHOME" ,		NMK_WEBHOME)
		.value("NMK_WEBREFRESH" ,	NMK_WEBREFRESH)
		.value("NMK_WEBSEARCH" ,	NMK_WEBSEARCH)
		.value("NMK_WEBSTOP" ,		NMK_WEBSTOP)
		.value("NMK_X" ,			NMK_X)
		.value("NMK_Y" ,			NMK_Y)
		.value("NMK_YEN" ,			NMK_YEN)
		.value("NMK_Z" ,			NMK_Z)
		.value("NMK_INVALIDE" ,		NMK_INVALIDE);
	class_<KeyboardEvent>("KeyboardEvent")
		.def_readwrite("type", &KeyboardEvent::type)
		.def_readwrite("value", &KeyboardEvent::value);
	//end HID 


	//Game play module begin............ TODO: add some function and property
	class_<GameWorld, SmartPtr<GameWorld>::Type>("GameWorld")
		.def("get_game_object_by_id", &GameWorld::get_game_object_by_id)
		.def("get_game_object_by_name", &GameWorld::get_game_object_by_name);
	//class_<GameWorldWrapper, boost::noncopyable,
	//	SmartPtr<GameWorldWrapper>::Type>("GameWorldWrapper")
	//	.def("get_game_object_by_id", &GameWorld::get_game_object_by_id)
	//	.def("get_game_object_by_name", &GameWorld::get_game_object_by_name);
	def("get_game_world", get_game_world, 
		return_value_policy<reference_existing_object>());



	class_<GameObject, SmartPtr<GameObject>::Type>("GameObject",
		init<U32, String>())
		.def("play_animation", &GameObject::play_animation)
		.def("set_transform", &GameObject::set_transform);
	class_<GameObjectWrapper, boost::noncopyable,
		SmartPtr<GameObjectWrapper>::Type>("GameObjectWrapper",
		init<U32, String>())
		.def("play_animation", &GameObject::play_animation)
		.def("set_transform", &GameObject::set_transform);

	class_<Property, SmartPtr<Property>::Type>("Property");
	class_<PropertyWrapper, boost::noncopyable,
		SmartPtr<PropertyWrapper>::Type>("PropertyWrapper");
	class_<IntegerProperty, bases<Property>>("IntegerProperty", init<int>());
	class_<StringProperty, bases<Property>>("StringProperty", init<String>());
	//Game play end

	//NECFile begin.........
	class_<NECMesh, SmartPtr<NECMesh>::Type>("NECMesh");
	//NECFile end........

	//GameResource module
	def("load_game_object_from_resource_file", load_game_object_from_resource_file);
	def("create_game_object_instance_from_resource_file", create_game_object_instance_from_resource_file);
	def("attach_resource_file", attach_resource_file);
	def("detach_resource_file", detach_resource_file);
	//GameResource end

	//Necromancer begin ......... TODO: add more functions
	def("end", end);
	def("attach_game_object", attach_game_object);
	//Necromancer end
}

namespace Necromancer{
	void init_wrap_modules(){
		PyImport_AppendInittab("Necromancer", &PyInit_Necromancer);
	}
}