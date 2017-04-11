#include "stdafx.h"
#include "RenderContext.h"

#include "VSProcess.h"
#include "FSProcess.h"
#include "PIL.h"
#include "PILShaderManager.h"

namespace Necromancer{
	RenderContext::~RenderContext(){
	}

	void RenderContext::attach_vs_process(U32 seq, VSProcess* vs_process){
		m_vs_process_map[seq] = vs_process->uid();
	}

	void RenderContext::attach_vs_process(U32 seq, U32 uid){
		m_vs_process_map[seq] = uid;
	}

	void RenderContext::attach_fs_process(U32 seq, FSProcess* fs_process){
		m_fs_process_map[seq] = fs_process->uid();
	}

	void RenderContext::attach_fs_process(U32 seq, U32 uid){
		m_fs_process_map[seq] = uid;
	}

	void  RenderContext::set_constant_buffer(U32 slot, const CBPtr& cb){
		m_cb_map[slot] = cb;
	}

	void  RenderContext::set_texture(U32 slot, const TexPtr& tex){
		m_tex_map[slot] = tex;
	}

	void RenderContext::generate_vertex_shader(){
		String vb_format = m_vertex_buffer->format();
		String vs_str = "";
		for(auto iter = m_vs_process_map.begin();
			iter != m_vs_process_map.end();++ iter)
		{
			if(iter != m_vs_process_map.begin()){
				vs_str += "_"; 
			}
			vs_str += std::to_string(iter->second);
		}

		PIL* pil = PIL::get_instance();
		PILShaderManager* shader_manager = pil->pil_shader_manager();
		m_vertex_shader = shader_manager->create_vertex_shader(
			vs_str.c_str(), vb_format.c_str());
	}

	void RenderContext::generate_fragment_shader(){
		String vs_output_format = m_vertex_shader->output_format();
		String fs_str = "";
		for(auto iter = m_fs_process_map.begin();
			iter != m_fs_process_map.end();++ iter)
		{
			if(iter != m_fs_process_map.begin()){
				fs_str += "_"; 
			}
			fs_str += std::to_string(iter->second);
		}

		PIL* pil = PIL::get_instance();
		PILShaderManager* shader_manager = pil->pil_shader_manager();
		m_fragment_shader = shader_manager->create_fragment_shader(
			fs_str.c_str(), vs_output_format.c_str());
	}

	void RenderContext::generate_shaders(){
		generate_vertex_shader();
		generate_fragment_shader();
	}

	const Map<U32, CBPtr>::Type& RenderContext::cb_map(){
		return m_cb_map;
	}

	const Map<U32, TexPtr>::Type& RenderContext::tex_map(){
		return m_tex_map;
	}
}