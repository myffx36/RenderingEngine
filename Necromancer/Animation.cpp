#include "stdafx.h"
#include "Animation.h"

#include "Exception.h"

namespace Necromancer{
	Animation::Animation(
		const String& name,
		U32 bone_number, U32 frame_number)
		:m_name(name), m_bone_number(bone_number),
		m_frame_number(frame_number)
	{
		m_matrices.resize(frame_number);
		for (U32 i = 0; i < frame_number; ++i){
			m_matrices[i].resize(bone_number);
		}
		m_frame_time.resize(frame_number);
	}

	Animation::~Animation(){

	}

	void Animation::set_matrix(U32 matrix_number,
		U32 frame_number, const Mat4& matrix)
	{
//#ifdef _DEBUG
//		if (matrix_number > m_matrices.size())
//			throw Exception("matrix number out of range in the animation");
//
//		if (frame_number > m_frame_number){
//			throw Exception("frame number out of range in the animation");
//		}
//#endif
		m_matrices[frame_number][matrix_number] = matrix;
	}

	Mat4 Animation::get_matrix(U32 matrix_number,
		U32 frame_number) const
	{
//#ifdef _DEBUG
//		if (matrix_number > m_matrices.size())
//			throw Exception("matrix number out of range in the animation");
//
//		if (frame_number > m_frame_number){
//			throw Exception("frame number out of range in the animation");
//		}
//#endif
		return m_matrices[frame_number][matrix_number];
	}

	F32 Animation::get_frame_time(U32 i) const{
//#ifdef _DEBUG
//		if (i > m_frame_number){
//			throw Exception("frame number out of range in the animation");
//		}
//#endif
		return m_frame_time[i];
	}

	void Animation::set_frame_time(U32 i, F32 time){
//#ifdef _DEBUG
//		if (i > m_frame_number){
//			throw Exception("frame number out of range in the animation");
//		}
//#endif
		m_frame_time[i] = time;
	}
}