#pragma once

#include "Mesh.h"
#include "CommonUtility.h"
#include "SkeletonInstance.h"

namespace Necromancer{
	class SoftwareDeformMesh : public Mesh{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(SkeletonInstancePtr, skeleton_instance);
	public:
		SoftwareDeformMesh(const SkeletonInstancePtr&, U32 number);

		virtual ~SoftwareDeformMesh();

		virtual void enable(RenderContext*);
		virtual void disable(RenderContext*);

		void set_matrix();
	private:
		void update();
	private:
		U32* m_skeleton_indices;
		F32* m_skeleton_weights;
		U32 m_vertex_number;
		F32* m_original_vertices;
		bool m_pos_flag, m_nor_flag, m_tan_flag;
		U32 m_vertex_size;
	};
}

