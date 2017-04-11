#pragma once

#include "Property.h"
#include "CommonUtility.h"

namespace Necromancer{
	class IntegerProperty:public Property{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(int, integer_value);
	public:
		IntegerProperty(int);

		virtual ~IntegerProperty();
	private:
	};
}

