#pragma once

#include "Container.hpp"
#include "CommonUtility.h"
#include "Property.h"

namespace Necromancer{
	class StringProperty:public Property{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, string_value);
	public:
		StringProperty(const String& value);

		virtual ~StringProperty();
	private:
	};
}

