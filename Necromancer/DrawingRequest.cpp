#include "stdafx.h"
#include "DrawingRequest.h"

namespace Necromancer{
	void DrawingRequest::init(){
		m_opaque_geometry_num = 0;
		m_transparent_geometry_num = 0;
		m_light_num = 0;
		m_camera = nullptr;
		m_opaque_geometrys.fill(nullptr);
		m_transparent_geometrys.fill(nullptr);
		m_lights.fill(Light());
	}

	void DrawingRequest::attach_opaque_geometry(
		const GeometryPtr& geometry)
	{
		m_opaque_geometrys[
			m_opaque_geometry_num++] = geometry;
	}

	void DrawingRequest::attach_transparent_geometry(
		const GeometryPtr& geometry)
	{
		m_transparent_geometrys[
			m_transparent_geometry_num++] = geometry;
	}

	void DrawingRequest::attach_light(const Light& light){
		m_lights[m_light_num ++] = light;
	}
}