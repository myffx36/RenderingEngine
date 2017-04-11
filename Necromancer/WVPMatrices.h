#pragma once

#include "CommonUtility.h"
#include "BasicMath.h"
#include "RenderingResource.h"

namespace Necromancer{
	struct _WVPMatrices{
		Mat4 wolrd, view, projection;
	};
	class WVPMatrices{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(_WVPMatrices, matrices);
	public:
		WVPMatrices();
		~WVPMatrices();

		void set_world(const Mat4&);
		void set_view(const Mat4&);
		void set_projection(const Mat4&);
		void enable();
		void disable();
		void update();
	private:
		static const U32 ks_cb_slot = 0;
		CBPtr m_matrices_cb;
	};
}

