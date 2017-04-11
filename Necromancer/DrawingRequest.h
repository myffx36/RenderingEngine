#pragma once

#include "Mesh.h"
#include "Geometry.h"
#include "Light.h"
#include "Camera.h"
#include "CommonUtility.h"
#include "Container.hpp"

namespace Necromancer{
	class DrawingRequest{
		static const int ks_max_mesh_num = 1024;
		static const int ks_max_light_num = 128;
	public:
		typedef Array<GeometryPtr, ks_max_mesh_num>::Type GeometryArray;
		typedef Array<Light, ks_max_light_num>::Type LightArray;
		DEF_DATA_MEMBER_AND_GETTER_SETTER(CameraPtr, camera);
		DEF_DATA_MEMBER_AND_GETTER(U32, opaque_geometry_num);
		DEF_DATA_MEMBER_AND_GETTER(U32, transparent_geometry_num);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, light_num);
	public:
		const GeometryArray& opaque_geometrys() const{
			return m_opaque_geometrys;
		}
		const GeometryArray& transparent_geometrys() const{
			return m_transparent_geometrys;
		}

		const LightArray& lights() const{
			return m_lights;
		}

		void attach_opaque_geometry(const GeometryPtr&);
		void attach_transparent_geometry(const GeometryPtr&);
		void attach_light(const Light&);

		void init();
	private:
		GeometryArray m_opaque_geometrys;
		GeometryArray m_transparent_geometrys;
		LightArray m_lights;
	};
}

