#pragma once

#include "Mesh.h"
#include "BasicMath.h"
#include "CommonUtility.h"
#include "RenderingResource.h"
#include "SkeletonInstance.h"

namespace Necromancer{
	class HardwareDeformMesh :public Mesh{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(CBPtr, skeleton_cb);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(SkeletonInstancePtr, skeleton_instance);
	public:
		HardwareDeformMesh();
		virtual ~HardwareDeformMesh();

		virtual void enable(RenderContext*);
		virtual void disable(RenderContext*);

		void set_matrix(U32 i, const Mat4& matrix);
	private:
		void update();
	private:
		static const U32 ks_max_matrix_num = 128;
		Mat4 m_matrices[ks_max_matrix_num];
		static const U32 ks_skeleton_cb_slot = 5;
		Mat4 m_transform_matrices[ks_max_matrix_num];
	};
}

