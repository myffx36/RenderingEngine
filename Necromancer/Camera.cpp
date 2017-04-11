#include "stdafx.h"
#include "Camera.h"
#include "RenderContext.h"

namespace Necromancer{
	Camera::Camera(){
	}

	Camera::~Camera(){
	}

	void Camera::update_projection_matrix(){
		m_projection = 
			perspective_projection_matrix(
			m_aspect, m_theta, m_znear, m_zfar);
		m_view_projection = 
			m_view * m_projection;
	}

	void Camera::update_view_matrix(){
		m_view = look_at_matrix_by_look_at_dir(
			m_eye, m_look_at, m_up);
		m_view_projection = 
			m_view * m_projection;
	}

	Camera::Camera(F32 aspect, F32 theta, F32 znear, F32 zfar,
		const Vec3& eye,
		const Vec3& look_at,
		const Vec3& up)
		:m_aspect(aspect), m_theta(theta), m_znear(znear), 
		m_zfar(zfar), m_eye(eye), m_look_at(look_at), m_up(up)
	{
		update_projection_matrix();
		
		update_view_matrix();
	}

	void Camera::enable(RenderContext* render_context){
		update_view_matrix();
		render_context->set_view_matrix(m_view);
		render_context->set_projection_matrix(m_projection);
	}

	void Camera::disable(RenderContext* render_context ){
	}
}