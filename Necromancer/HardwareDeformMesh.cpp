#include "stdafx.h"
#include "HardwareDeformMesh.h"

#include "RenderContext.h"
#include "VSGenerator.h"
#include "PIL.h"
#include "PILLLGI.h"

namespace Necromancer{
	HardwareDeformMesh::HardwareDeformMesh(){
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		m_skeleton_cb = llgi->create_constant_buffer(
			sizeof(Mat4) * ks_skeleton_max_matrix_num, nullptr);

		for (U32 i = 0; i < ks_skeleton_max_matrix_num; ++i){
			m_transform_matrices[i] = translate_matrix(.0f, .0f, .0f);
		}
	}

	HardwareDeformMesh::~HardwareDeformMesh(){

	}

	void HardwareDeformMesh::update(){
		U32 skeleton_num = m_skeleton_instance->bone_num();
		for (U32 i = 0; i < skeleton_num; ++i){
			//m_transform_matrices[i] = m_skeleton_instance->get_matrix(i);
			//m_transform_matrices[i] = translate_matrix(0.0f, 0.0f, 0.0f);
			m_transform_matrices[i] = m_matrices[i] * m_skeleton_instance->get_matrix(i);
			//m_transform_matrices[i] = m_skeleton_instance->get_matrix(i) * m_matrices[i];
			//m_transform_matrices[i] = inverse_matrix(m_transform_matrices[i]);
		}
		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		llgi->update_constant_buffer_object(
			m_skeleton_cb, m_transform_matrices);
	}

	void HardwareDeformMesh::enable(RenderContext* rc){
		Mesh::enable(rc);
		rc->attach_vs_process(40, skining_transform_vs_uid);
		rc->set_constant_buffer(ks_skeleton_cb_slot, m_skeleton_cb);
		update();
	}

	void HardwareDeformMesh::disable(RenderContext* rc){
		Mesh::disable(rc);
	}

	void HardwareDeformMesh::set_matrix(U32 i, const Mat4& matrix){
		m_matrices[i] = matrix;
	}
}