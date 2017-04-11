#include "stdafx.h"
#include "Mesh.h"
#include "PIL.h"
#include "PILLLGI.h"
#include "RenderContext.h"
#include "VSGenerator.h"

namespace Necromancer{
	Mesh::Mesh(){
		m_geometry_type = NECROMANCER_GEOMETRY_MESH;
	}

	Mesh::~Mesh(){

	}

	void Mesh::register_cb(U32 i, const CBPtr& cb){
		m_cb_map[i] = cb;
	}

	const Map<U32, CBPtr>::Type& Mesh::cb_map() const{
		return m_cb_map;
	}

	void Mesh::enable(RenderContext* context){
		context->set_vertex_buffer(m_vertex_buffer);
		context->set_index_buffer(m_index_buffer);
		for(auto iter = m_cb_map.begin();iter != m_cb_map.end();++ iter){
			context->set_constant_buffer(iter->first, iter->second);
		}
		context->set_geometry_type(m_geometry_type);
		context->attach_vs_process(50, perspective_projection_vs_uid);
	}

	void Mesh::disable(RenderContext* context){
		context->set_vertex_buffer(nullptr);
		context->set_index_buffer(nullptr);
		for(auto iter = m_cb_map.begin();iter != m_cb_map.end();++ iter){
			context->set_constant_buffer(iter->first, nullptr);
		}
	}
}