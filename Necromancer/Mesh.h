#pragma once

#include "Container.hpp"
#include "RenderingResource.h"
#include "CommonUtility.h"
#include "BasicMath.h"

namespace Necromancer{
	class RenderContext;
	class Mesh{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(VBPtr, vertex_buffer);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(IBPtr, index_buffer);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(GeometryType, geometry_type);
	public:
		Mesh();
		virtual ~Mesh();

		void register_cb(U32, const CBPtr&);
		const Map<U32, CBPtr>::Type& cb_map() const;

		virtual void enable(RenderContext*);
		virtual void disable(RenderContext*);
	private:
		Map<U32, CBPtr>::Type m_cb_map;
	};

	typedef SmartPtr<Mesh>::Type MeshPtr;
}

