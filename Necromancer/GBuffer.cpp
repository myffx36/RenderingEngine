#include "stdafx.h"
#include "GBuffer.h"

#include "PIL.h"
#include "PILLLGI.h"

namespace Necromancer{
	GBuffer::GBuffer(U32 width, U32 height)
		:m_width(width), m_height(height)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		m_depth_buffer = llgi->create_readable_depth_stencil(
			m_width, m_height);
		for (int i = 0; i < ks_texture_number; ++i){
			m_textures[i] = llgi->create_renderable_texture_2d(
				m_width, m_height, NECROMANCER_TEXTURE_R8G8B8A8);
		}
	}

	GBuffer::~GBuffer(){
	}

	void GBuffer::bind_to_read(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		BindFlag flag = (BindFlag) (BIND_TO_FS | BIND_TO_CS);
		llgi->enable_texture(ks_depth_buffer_slot, m_depth_buffer, flag);
		for (int i = 0; i < ks_texture_number; ++i){
			llgi->enable_texture(ks_texture_start_slot + i, m_textures[i], flag);
		}
	}

	void GBuffer::bind_to_write(){
		float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		RTPtr rts[ks_texture_number];
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		for (int i = 0; i < ks_texture_number; ++i){
			llgi->clear_render_target(m_textures[i], black);
			rts[i] = m_textures[i];
		}
		llgi->clear_depth_stencil(m_depth_buffer);
		llgi->set_render_targets(ks_texture_number, rts, m_depth_buffer);
	}

	void GBuffer::unbind_to_read(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		BindFlag flag = (BindFlag) (BIND_TO_FS | BIND_TO_CS);
		llgi->disable_texture(ks_depth_buffer_slot, nullptr, flag);
		for (int i = 0; i < ks_texture_number; ++i){
			llgi->disable_texture(ks_texture_start_slot + i, nullptr, flag);
		}
	}

	void GBuffer::unbind_to_write(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->set_to_default_render_target();
	}
}