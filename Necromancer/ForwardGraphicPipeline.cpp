#include "stdafx.h"
#include "ForwardGraphicPipeline.h"

#include "DrawingRequest.h"
#include "RenderContext.h"
#include "FSGenerator.h"
#include "ShadowMapManager.h"

namespace Necromancer{
	ForwardGraphicPipeline::ForwardGraphicPipeline(
		Configuration* config,PILLLGI* llgi)
	{
		m_llgi = llgi;

		m_light_cb = m_llgi->create_constant_buffer(
			ks_max_light_num  * sizeof(Light), nullptr);
		m_light_number_cb = m_llgi->create_constant_buffer(
			sizeof(U32) * 4, nullptr);

		m_shadow_map_manager = new ShadowMapManager(config);
	}

	ForwardGraphicPipeline::~ForwardGraphicPipeline(){
		delete m_shadow_map_manager;
	}

	void ForwardGraphicPipeline::process_drawing_request(DrawingRequest* request){
		CameraPtr camera = request->camera();
		//m_matrices.set_view(camera->view());
		//m_matrices.set_projection(camera->projection());

		const auto& lights = request->lights();
		U32 light_num = request->light_num();
		int geometry_num = request->geometry_num();
		const DrawingRequest::GeometryArray& geometry_array = 
			request->geometrys();

		//m_shadow_map_manager->clean_status();
		//for (U32 i = 0; i < lights.size(); ++i){
		//	m_shadow_map_manager->generate_shadow_map(
		//		i, lights[i], geometry_array, geometry_num);
		//}
		//m_shadow_map_manager->enable_shadow_maps();

		set_lights(lights.data(), light_num);

		F32 black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_llgi->clear_default_render_target(black);

		for(int i = 0;i < geometry_num;++ i){
			draw_geometry(geometry_array[i], camera);
		}

		//m_shadow_map_manager->disable_shadow_maps();
		//move to rendering engine
		//m_llgi->swap_buffer();
	}

	void ForwardGraphicPipeline::draw_geometry(
		const GeometryPtr& geometry, const CameraPtr& camera)
	{
		RenderContext* render_context = new RenderContext();
		camera->enable(render_context);
		geometry->enable(render_context);
		render_context->attach_fs_process(80, lighting_fs_uid);
		apply_render_context(render_context);
		m_llgi->draw();
		camera->disable(render_context);
		geometry->disable(render_context);

		delete render_context;
	}

	void ForwardGraphicPipeline::set_lights(const Light* lights, U32 num){
		U32 tmp_array[] = {
			num, num, num, num
		};
		m_llgi->update_constant_buffer_object(m_light_cb, lights);
		m_llgi->update_constant_buffer_object(m_light_number_cb, tmp_array);
		m_llgi->enable_constant_buffer(ks_light_cb_num_slot, m_light_number_cb);
		m_llgi->enable_constant_buffer(ks_light_cb_slot, m_light_cb);
	}

	void ForwardGraphicPipeline::apply_render_context(RenderContext* render_context){
		render_context->generate_shaders();
		m_llgi->enable_vertex_buffer(render_context->vertex_buffer());
		m_llgi->enable_index_buffer(render_context->index_buffer());
		m_llgi->set_geometry_type(render_context->geometry_type());
		m_llgi->enable_vertex_shader(render_context->vertex_shader());
		m_llgi->enable_fragment_shader(render_context->fragment_shader());

		const auto& cb_map = render_context->cb_map();
		const auto& tex_map = render_context->tex_map();

		for(auto iter = cb_map.begin(); iter != cb_map.end();++ iter){
			m_llgi->enable_constant_buffer(iter->first, iter->second);
		}

		for(auto iter = tex_map.begin(); iter != tex_map.end();++ iter){
			m_llgi->enable_texture(iter->first, iter->second);
		}

		m_matrices.set_world(render_context->world_matrix());
		m_matrices.set_view(render_context->view_matrix());
		m_matrices.set_projection(render_context->projection_matrix());
		m_matrices.update();
		m_matrices.enable();
	}
}