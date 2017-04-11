#include "stdafx.h"
#include "Geometry.h"

#include "RenderContext.h"

namespace Necromancer{
	Geometry::Geometry(bool opaque) :m_is_opaque(opaque)
	{

	}

	Geometry::~Geometry(){
	}

	void Geometry::enable(RenderContext* context){
		m_mesh->enable(context);
		//if(m_skeleton != nullptr) m_skeleton->enable(context);
		if(m_material != nullptr) m_material->enable(context);
		context->set_world_matrix(m_world);
	}

	void Geometry::disable(RenderContext* context){
		m_mesh->disable(context);
		//if(m_skeleton != nullptr) m_skeleton->disable(context);
		if(m_material != nullptr) m_material->disable(context);
	}

	void Geometry::set_opaque(bool opaque){
		m_is_opaque = opaque;
	}

	bool Geometry::is_opaque() const{
		return m_is_opaque;
	}
}