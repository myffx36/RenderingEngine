#include "stdafx.h"
#include "DynamicLineSetMesh.h"

#include "RenderContext.h"
#include "FSGenerator.h"
#include "PIL.h"
#include "PILLLGI.h"

namespace Necromancer{
	namespace{
		GraphicStatePtr create_default_graphic_state(){
			SamplerStateDesc sampler_state_desc;
			sampler_state_desc.max_anisotropic = 0;
			sampler_state_desc.sampler_filter =
				NECROMANCER_TEXTURE_FILTER_LINEAR;
			sampler_state_desc.texture_address_mode =
				NECROMANCER_TEXTURE_ADDRESS_BORDER;
			RasterizerStateDesc rasterizer_state_desc;
			rasterizer_state_desc.antialiased_line = true;
			rasterizer_state_desc.cull_mode =
				NECROMANCER_CULL_NONE;
			rasterizer_state_desc.fill_mode =
				NECROMANCER_FILL_SOLID;
			rasterizer_state_desc.multisample = false;
			DepthStencilStateDesc depth_stencil_state_desc;
			depth_stencil_state_desc.depth_write = false;
			depth_stencil_state_desc.comparison =
				NECROMANCER_DEPTH_COMPARISON_ALWAYS;
			depth_stencil_state_desc.depth_testing = true;
			depth_stencil_state_desc.stencil_testing = false;
			AlphaStateDesc alpha_state_desc;
			alpha_state_desc.enable = false;
			PIL* pil = PIL::get_instance();
			PILLLGI* llgi = pil->pil_llgi();

			GraphicStatePtr state = llgi->create_graphic_state(
				sampler_state_desc, rasterizer_state_desc,
				depth_stencil_state_desc, alpha_state_desc);

			return state;
		}
	}

	DynamicLineSetMesh::DynamicLineSetMesh()
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();

		VBPtr vb = llgi->create_vertex_buffer(
			ks_max_line_num * ks_line_data_size * sizeof(float),
			7 * sizeof(float), 0, "PC", m_data,
			NM_BUFFER_USAGE_DYNAMIC);

		U32 indices[ks_max_line_num * 2];
		for (int i = 0; i < ks_max_line_num * 2; ++i){
			indices[i] = i;
		}
		IBPtr ib = llgi->create_index_buffer(ks_max_line_num * 2, indices);

		set_vertex_buffer(vb);
		set_index_buffer(ib);
		set_geometry_type(NECROMANCER_GEOMETRY_LINES);

		m_current_line_num = 0;
		m_graphic_state = create_default_graphic_state();
	}

	DynamicLineSetMesh::~DynamicLineSetMesh(){
	}


	void DynamicLineSetMesh::enable(RenderContext* rc){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		update_vertices();
		Mesh::enable(rc);
		llgi->enable_graphic_state(m_graphic_state);
		rc->attach_fs_process(60, red_fs_uid);
	}

	void DynamicLineSetMesh::disable(RenderContext* rc){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		Mesh::disable(rc);
		llgi->disable_graphic_state(m_graphic_state);
	}

	void DynamicLineSetMesh::update_vertices(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		float* vb_data = (float*) llgi->map(vertex_buffer());
		memcpy_s(vb_data, m_current_line_num * ks_line_data_size * sizeof(float),
			m_data, m_current_line_num * ks_line_data_size * sizeof(float));
		llgi->unmap(vertex_buffer());
		IBPtr ib = index_buffer();
		ib->set_index_number(m_current_line_num * 2);
		clean_up();
	}

	void DynamicLineSetMesh::attach_line(const Vec3& start,
		const Vec3& end, const Vec4& color)
	{
		m_data[m_current_line_num * ks_line_data_size + 0] = start.x;
		m_data[m_current_line_num * ks_line_data_size + 1] = start.y;
		m_data[m_current_line_num * ks_line_data_size + 2] = start.z;
		m_data[m_current_line_num * ks_line_data_size + 3] = color.x;
		m_data[m_current_line_num * ks_line_data_size + 4] = color.y;
		m_data[m_current_line_num * ks_line_data_size + 5] = color.z;
		m_data[m_current_line_num * ks_line_data_size + 6] = color.w;
		m_data[m_current_line_num * ks_line_data_size + 7] = end.x;
		m_data[m_current_line_num * ks_line_data_size + 8] = end.y;
		m_data[m_current_line_num * ks_line_data_size + 9] = end.z;
		m_data[m_current_line_num * ks_line_data_size + 10] = color.x;
		m_data[m_current_line_num * ks_line_data_size + 11] = color.y;
		m_data[m_current_line_num * ks_line_data_size + 12] = color.z;
		m_data[m_current_line_num * ks_line_data_size + 13] = color.w;

		++m_current_line_num;
	}

	void DynamicLineSetMesh::attach_line(const Vec3& start, Vec4& start_color,
		const Vec3& end, const Vec4& end_color)
	{
		m_data[m_current_line_num * ks_line_data_size + 0] = start.x;
		m_data[m_current_line_num * ks_line_data_size + 1] = start.y;
		m_data[m_current_line_num * ks_line_data_size + 2] = start.z;
		m_data[m_current_line_num * ks_line_data_size + 3] = start_color.x;
		m_data[m_current_line_num * ks_line_data_size + 4] = start_color.y;
		m_data[m_current_line_num * ks_line_data_size + 5] = start_color.z;
		m_data[m_current_line_num * ks_line_data_size + 6] = start_color.w;
		m_data[m_current_line_num * ks_line_data_size + 7] = end.x;
		m_data[m_current_line_num * ks_line_data_size + 8] = end.y;
		m_data[m_current_line_num * ks_line_data_size + 9] = end.z;
		m_data[m_current_line_num * ks_line_data_size + 10] = end_color.x;
		m_data[m_current_line_num * ks_line_data_size + 11] = end_color.y;
		m_data[m_current_line_num * ks_line_data_size + 12] = end_color.z;
		m_data[m_current_line_num * ks_line_data_size + 13] = end_color.w;

		++m_current_line_num;
	}

	void DynamicLineSetMesh::clean_up(){
		m_current_line_num = 0;
	}
}