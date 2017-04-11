#include "stdafx.h"
#include "DeferredGraphicPipeline.h"

#include "DrawingRequest.h"
#include "RenderContext.h"
#include "PIL.h"
#include "PILLLGI.h"
#include "FSGenerator.h"
#include "GBuffer.h"
#include "DeferredShader.h"
#include "TileBasedDeferredShader.h"
#include "Configuration.h"
#include "ShadowMaskGenerator.h"

namespace Necromancer{
	DeferredGraphicPipeline::DeferredGraphicPipeline(
		Configuration* config, PILLLGI* llgi)
	{
		U32 width = config->get_integer_value("Screen", "width");
		U32 height = config->get_integer_value("Screen", "height");
		m_g_buffer = new GBuffer(width, height);
		m_deferred_shader = new TileBasedDeferredShader(width, height);
		m_shadow_mask_generator = new ShadowMaskGenerator(config);
	}


	DeferredGraphicPipeline::~DeferredGraphicPipeline(){
		if (m_shadow_mask_generator) delete m_shadow_mask_generator;
		if (m_g_buffer) delete m_g_buffer;
		if (m_deferred_shader) delete m_deferred_shader;
	}

	void DeferredGraphicPipeline::render_to_g_buffer(
		DrawingRequest* request)
	{
		CameraPtr camera = request->camera();
		U32 geometry_num = request->opaque_geometry_num();
		const DrawingRequest::GeometryArray& geometry_array =
			request->opaque_geometrys();
		m_g_buffer->bind_to_write();
		for (U32 i = 0; i < geometry_num; ++i){
			draw_geometry_to_g_buffer(
				geometry_array[i], camera);
		}
		m_g_buffer->unbind_to_write();
	}

	void DeferredGraphicPipeline::draw_geometry_to_g_buffer(
		const GeometryPtr& geometry, const CameraPtr& camera)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		RenderContext* render_context = new RenderContext();
		camera->enable(render_context);
		geometry->enable(render_context);
		render_context->attach_fs_process(80, g_buffer_fs_uid);
		apply_render_context(render_context);
		llgi->draw();
		camera->disable(render_context);
		geometry->disable(render_context);

		delete render_context;
	}

	void DeferredGraphicPipeline::process_drawing_request(
		DrawingRequest* request)
	{
		render_to_g_buffer(request);
		m_shadow_mask_generator->generate_shadow_mask(
			request, m_g_buffer);
		m_shadow_mask_generator->enable_shadow_mask();
		deferred_shading(request);
		m_shadow_mask_generator->disable_shadow_mask();
		draw_all_transparent_objects(request);
	}

	void DeferredGraphicPipeline::apply_render_context(
		RenderContext* render_context)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		render_context->generate_shaders();
		llgi->enable_vertex_buffer(render_context->vertex_buffer());
		llgi->enable_index_buffer(render_context->index_buffer());
		llgi->set_geometry_type(render_context->geometry_type());
		llgi->enable_vertex_shader(render_context->vertex_shader());
		llgi->enable_fragment_shader(render_context->fragment_shader());

		const auto& cb_map = render_context->cb_map();
		const auto& tex_map = render_context->tex_map();

		for (auto iter = cb_map.begin(); iter != cb_map.end(); ++iter){
			llgi->enable_constant_buffer(iter->first, iter->second);
		}

		for (auto iter = tex_map.begin(); iter != tex_map.end(); ++iter){
			llgi->enable_texture(iter->first, iter->second);
		}

		m_matrices.set_world(render_context->world_matrix());
		m_matrices.set_view(render_context->view_matrix());
		m_matrices.set_projection(render_context->projection_matrix());
		m_matrices.update();
		m_matrices.enable();
	}

	void DeferredGraphicPipeline::deferred_shading(
		DrawingRequest* request)
	{
		CameraPtr camera = request->camera();
		const auto& lights = request->lights();
		U32 light_num = request->light_num();
		m_deferred_shader->deferred_shading(lights.data(),
			light_num, m_g_buffer, true, camera);
	}

	void DeferredGraphicPipeline::generate_shadow_mask_map(
		DrawingRequest* request)
	{
	}

	void DeferredGraphicPipeline::draw_all_transparent_objects(
		DrawingRequest* request)
	{
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->set_to_default_render_target();

		CameraPtr camera = request->camera();
		U32 geometry_num = request->transparent_geometry_num();
		const DrawingRequest::GeometryArray& geometry_array =
			request->transparent_geometrys();
		for (U32 i = 0; i < geometry_num; ++i){
			RenderContext render_context;
			camera->enable(&render_context);
			geometry_array[i]->enable(&render_context);
			//render_context.attach_fs_process(80, g_buffer_fs_uid);
			apply_render_context(&render_context);
			llgi->draw();
			camera->disable(&render_context);
			geometry_array[i]->disable(&render_context);
		}
	}
}