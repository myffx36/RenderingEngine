#include "stdafx.h"
#include "SoftwareDeformMesh.h"

#include "PIL.h"
#include "PILLLGI.h"

namespace Necromancer{
	SoftwareDeformMesh::SoftwareDeformMesh(
		const SkeletonInstancePtr& skeleton_instance, U32 vertex_number)
	{
		m_vertex_number = vertex_number;
		m_skeleton_indices = new U32[vertex_number * 4];
		m_skeleton_weights = new F32[vertex_number * 4];
	}

	SoftwareDeformMesh::~SoftwareDeformMesh(){
		delete[] m_skeleton_indices;
		delete[] m_skeleton_weights;
	}

	void SoftwareDeformMesh::enable(RenderContext* rc){
		VBPtr vb = vertex_buffer();
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		float* vertices = static_cast<float*>(llgi->map(vb));
		//process

		for (U32 i = 0; i < m_vertex_number; ++i){
			for (U32 j = 0; j < 4; ++j){
				if (m_skeleton_indices[i * 4 + j] > 255)
					continue;

				Vec3* position = 
					reinterpret_cast<Vec3*>(vertices + (m_vertex_size * i));
				if (m_tan_flag){

				}

				if (m_nor_flag){

				}
			}


		}

		llgi->unmap(vb);
	}

	void SoftwareDeformMesh::disable(RenderContext* rc){
	}

	void SoftwareDeformMesh::update(){
	}
}