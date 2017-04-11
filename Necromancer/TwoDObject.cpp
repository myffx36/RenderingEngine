#include "stdafx.h"
#include "TwoDObject.h"

#include "PIL.h"
#include "PILLLGI.h"

namespace Necromancer{

	namespace{
		const char* vs_str = "cbuffer matrices:register(b0){\n\
							 float4x4 world;\n\
							 float4x4 projection;\n\
							 }\n\
							 \n\
							 struct PSINPUT{\n\
							 float4 t_pos:SV_POSITION;\n\
							 float2 tex:TEXCOORD0;\n\
							 };\n\
							 \n\
							 struct VSINPUT{\n\
							 float3 position:POSITION;\n\
							 float2 tex:TEXCOORD0;\n\
							 };\n\
							 \n\
							 PSINPUT main(VSINPUT input){\n\
							 PSINPUT output = (PSINPUT)0;\n\
							 \n\
							 output.tex = input.tex;\n\
							 output.t_pos = mul(world, float4(input.position, 1.0f));\n\
							 output.t_pos = mul(projection, output.t_pos);\n\
							 //output.t_pos = float4(input.position, 1.0f);\n\
							 \n\
							 return output;\n\
							 }";
		const char* fs_str = "Texture2D<float4> texture_slot0 : register( t0 );\n\
							 SamplerState samLinear : register( s0 );\n\
							 \n\
							 float4 main(float4 v_pos:SV_POSITION, float2 tex:TEXCOORD0) : SV_TARGET\n\
							 {\n\
							 return texture_slot0.Sample(samLinear, tex);\n\
							 }";

		struct TwoDConstantBufferStruct{
			Mat4 world;
			Mat4 projection;
		};

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

	WeakPtr<VertexShader>::Type TwoDObject::s_vertex_shader;
	WeakPtr<FragmentShader>::Type TwoDObject::s_fragment_shader;
	WeakPtr<VertexBuffer>::Type TwoDObject::s_vertex_buffer;
	WeakPtr<IndexBuffer>::Type TwoDObject::s_index_buffer;
	WeakPtr<ConstantBuffer>::Type TwoDObject::s_constant_buffer;
	WeakPtr<GraphicState>::Type TwoDObject::s_graphic_state;

	TwoDObject::TwoDObject(const Tex2DPtr& texture, 
		const VBPtr& vb, const IBPtr& ib,
		const VSPtr& vs, const FSPtr& fs)
		:m_vertex_shader(vs), m_fragment_shader(fs),
		m_vertex_buffer(vb), m_index_buffer(ib), m_texture(texture)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();

		if(!m_vertex_buffer){
			m_vertex_buffer = s_vertex_buffer.lock();
			if(!m_vertex_buffer){
				float vertices[] = {
					-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
					0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
					0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
					-0.5f, -0.5f, 0.0f, 0.0f, 1.0f};
				m_vertex_buffer = llgi->create_vertex_buffer(
					sizeof(vertices), 5 * sizeof(float), 0, "P2", vertices);

				s_vertex_buffer = m_vertex_buffer;
			}
		}

		if(!m_index_buffer){
			m_index_buffer = s_index_buffer.lock();
			if(!m_index_buffer){
				U32 indices[] = {0, 1, 2, 3, 0, 2};
				m_index_buffer = llgi->create_index_buffer(
					6, indices);

				s_index_buffer = m_index_buffer;
			}
		}

		if(!m_vertex_shader){
			m_vertex_shader = s_vertex_shader.lock();
			if(!m_vertex_shader){
				m_vertex_shader = llgi->create_vertex_shader(
					vs_str, "nofile");

				s_vertex_shader = m_vertex_shader;
			}
		}

		if(!m_fragment_shader){
			m_fragment_shader = s_fragment_shader.lock();
			if(!m_fragment_shader){
				m_fragment_shader = llgi->create_fragment_shader(
					fs_str, "nofile");

				s_fragment_shader = m_fragment_shader;
			}
		}

		m_constant_buffer = s_constant_buffer.lock();
		if(!m_constant_buffer){
			m_constant_buffer = llgi->create_constant_buffer(
				sizeof(TwoDConstantBufferStruct), nullptr);

			s_constant_buffer = m_constant_buffer;
		}

		m_graphic_state = s_graphic_state.lock();
		if (!m_graphic_state){
			m_graphic_state = create_default_graphic_state();

			s_graphic_state = m_graphic_state;
		}

		U32 screen_width = pil->screen_width();
		U32 screen_height = pil->screen_height();
		m_projection = orthographic_projection_matrix(
			screen_width / -2.0f, screen_width / 2.0f, 
			screen_height / -2.0f, screen_height / 2.0f, 0, 1.0f);
		//m_projection = orthographic_projection_matrix(
		//	0.0f, screen_width / 1.0f,
		//	0.0f, screen_height / 1.0f, 0.0f, 1.0f);

		m_position = Vec2(0.0f, 0.0f);
		m_size = Vec2(1.0f, 1.0f);
		calculate_world_matrix();
	}

	TwoDObject::~TwoDObject(){
	}

	void TwoDObject::enable(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();

		calculate_world_matrix();
		TwoDConstantBufferStruct constant_buffer_value;
		constant_buffer_value.world = m_world;
		constant_buffer_value.projection = m_projection;
		llgi->update_constant_buffer_object(
			m_constant_buffer, &constant_buffer_value);

		llgi->enable_vertex_buffer(m_vertex_buffer);
		llgi->enable_index_buffer(m_index_buffer);
		llgi->set_geometry_type(NECROMANCER_GEOMETRY_MESH);
		llgi->enable_vertex_shader(m_vertex_shader);
		llgi->enable_fragment_shader(m_fragment_shader);
		llgi->enable_constant_buffer(ks_cb_slot, m_constant_buffer);
		llgi->enable_texture(ks_texture_slot, m_texture);
		llgi->enable_graphic_state(m_graphic_state);
	}

	void TwoDObject::disable(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();

		//llgi->enable_vertex_buffer(nullptr);
		//llgi->enable_index_buffer(nullptr);
		//llgi->enable_vertex_shader(nullptr);
		//llgi->enable_fragment_shader(nullptr);
		//llgi->enable_constant_buffer(ks_cb_slot, nullptr);
		//llgi->enable_texture(ks_texture_slot, nullptr);

		llgi->disable_graphic_state(m_graphic_state);
	}

	void TwoDObject::draw(){
		update();
		enable();
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->draw();
		disable();
	}

	void TwoDObject::calculate_world_matrix(){
		//m_world = translate_matrix(m_position.x, m_position.y, 0.0f);
		m_world = scale_matrix(m_size.x, m_size.y, 1.0f) * 
			translate_matrix(m_position.x, m_position.y, 0.0f);
	}

	void TwoDObject::update(){

	}
}