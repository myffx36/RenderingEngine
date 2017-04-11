#pragma once

#include "Mesh.h"
#include "Material.h"
//#include "Skeleton.h"
#include "CommonUtility.h"


namespace Necromancer{
	class Geometry{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(MeshPtr, mesh);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(MaterialPtr, material);
		//DEF_DATA_MEMBER_AND_GETTER_SETTER(SkeletonPtr, skeleton);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Mat4, world);
	public:
		Geometry(bool is_opaque = true);
		virtual ~Geometry();

		virtual void enable(RenderContext* context);
		virtual void disable(RenderContext* context);

		bool is_opaque() const;

		void set_opaque(bool opaque);
	private:
		bool m_is_opaque;
	};

	typedef SmartPtr<Geometry>::Type GeometryPtr;
}

