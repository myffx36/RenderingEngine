#pragma once

#include "Container.hpp"
#include "BasicMath.h"
#include "CommonUtility.h"

namespace Necromancer{
	enum LightType{
		NM_LIGHT_POINT = 0,
		NM_LIGHT_DIRECTIONAL = 1,
		NM_LIGHT_SPOT = 2
	};
	class Light{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec4, position);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec4, direction);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec4, color);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec4, params);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec4, other_info);
	public:
		~Light();

		inline float theta() const{
			float radiance = acosf(m_other_info[1]);
			float theta = radiance / 3.14f * 180.0f;
			return theta;
		}
		//…Ë÷√thetaΩ«  degree
		//0 - 180
		inline void set_theta(float theta){
			float radiance = theta / 180.0f * 3.14f;
			m_other_info[1] = cosf(radiance);
		}

		inline LightType light_type() const{
			int x = *((int*)&m_other_info[0]);
			return (LightType)x;
		}

		inline void set_light_type(LightType type){
			int tmp = (int)type;
			m_other_info[0] = *((float*)&tmp);

		}
	private:
	};
}

