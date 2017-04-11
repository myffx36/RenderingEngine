#pragma once

#include "Container.hpp"
#include "CommonUtility.h"
#include "BasicMath.h"

namespace Necromancer{
	class RenderContext;
	class Camera{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Mat4, projection);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Mat4, view);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Mat4, view_projection);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(F32, aspect);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(F32, theta);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(F32, znear);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(F32, zfar);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec3, eye);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec3, look_at);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec3, up);
	public:
		Camera(F32 aspect, F32 theta, F32 znear, F32 zfar,
			const Vec3& eye, const Vec3& look_at, const Vec3& up);
		Camera();
		~Camera();

		void update_view_matrix();
		void update_projection_matrix();

		virtual void enable(RenderContext*);
		virtual void disable(RenderContext*);
	private:
	};

	typedef SmartPtr<Camera>::Type CameraPtr;
}

