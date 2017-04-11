#include "stdafx.h"
#include "WVPMatrices.h"

#include "PIL.h"
#include "PILLLGI.h"

namespace Necromancer{
	WVPMatrices::WVPMatrices(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		m_matrices_cb = llgi->create_constant_buffer(
			sizeof(_WVPMatrices), nullptr);
	}

	WVPMatrices::~WVPMatrices(){
	}

	void WVPMatrices::set_world(const Mat4& world){
		m_matrices.wolrd = world;
	}

	void WVPMatrices::set_view(const Mat4& view){
		m_matrices.view = view;
	}

	void WVPMatrices::set_projection(const Mat4& projection){
		m_matrices.projection = projection;
	}

	void WVPMatrices::enable(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->enable_constant_buffer(ks_cb_slot, m_matrices_cb);
	}

	void WVPMatrices::disable(){
		CBPtr null_ptr((ConstantBuffer*)nullptr);
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->enable_constant_buffer(ks_cb_slot, null_ptr);
	}

	void WVPMatrices::update(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->update_constant_buffer_object(
			m_matrices_cb, &m_matrices);
	}
}