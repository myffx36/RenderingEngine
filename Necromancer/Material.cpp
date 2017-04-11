#include "stdafx.h"
#include "Material.h"
#include "RenderContext.h"

#include "Necromancer.h"
#include "PIL.h"
#include "PILLLGI.h"
#include "FSGenerator.h"

namespace Necromancer{
	namespace{
		struct MaterialParameters{
			Vec4 diffuse;
			Vec4 ambient;
			Vec4 spcular;
			Vec4 other_parameters;
		};
	}

	Material::Material()
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		m_cb = llgi->create_constant_buffer(
			sizeof(MaterialParameters), nullptr);
	}

	Material::~Material(){

	}

	void Material::enable(RenderContext* context){
		MaterialParameters parameters;
		parameters.ambient = Vec4(ambient(), 1.0f);
		parameters.diffuse = Vec4(diffuse(), 1.0f);
		parameters.spcular = Vec4(specular(), 1.0f);
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->update_constant_buffer_object(m_cb, &parameters);
		//context->set_constant_buffer(ks_cb_slot, m_cb);

		for(auto iter = m_fs_process_bindings.begin(); 
			iter != m_fs_process_bindings.end();++ iter)
		{
			context->attach_fs_process(iter->seq, iter->uid);
		}

		for(auto iter = m_texture_bindings.begin();
			iter != m_texture_bindings.end();++ iter)
		{
			context->set_texture(iter->slot, iter->texture);
		}

		//context->attach_fs_process(60, red_fs_uid);
		if (m_texture_bindings.size() != 0){
			context->attach_fs_process(50, texture_maping_fs_uid);
		}
		//context->attach_fs_process(60, lighting_fs_uid);
	}

	void Material::disable(RenderContext* context){
		//TODO:
		//PIL* pil = PIL::get_instance();
		//PILLLGI* llgi = pil->pil_llgi();
		//llgi->disable_constant_buffer(ks_cb_slot, m_cb);
	}

	void Material::attach_texture(U32 i, const TexPtr& texture){
		TextureBinding bind;
		bind.slot = i;
		bind.texture = texture;
		m_texture_bindings.push_back(bind);
	}
}