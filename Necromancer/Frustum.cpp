#include "stdafx.h"
#include "Frustum.h"

namespace Necromancer{
	Frustum::Frustum(const Mat4& matrix){
		Vec4 c1, c2, c3, c4;
		for(int i = 0;i < 4;++ i){
			c1[i] = matrix[i][0];
			c2[i] = matrix[i][1];
			c3[i] = matrix[i][2];
			c4[i] = matrix[i][3];
		}

		m_right = c4 - c1;
		m_left = c4 + c1;
		m_top = c4 - c2;
		m_down = c4 + c2;
		m_f = c4 - c3;
		m_n = c4 + c3;

		m_right = m_right / length(m_right.xyz);
		m_left = m_left / length(m_left.xyz);
		m_top = m_top / length(m_top.xyz);
		m_down = m_down / length(m_down.xyz);
		m_f = m_f / length(m_f.xyz);
		m_n = m_n / length(m_n.xyz);
	}
}