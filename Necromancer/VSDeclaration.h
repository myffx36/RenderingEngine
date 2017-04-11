#pragma once

#include "Container.hpp"
#include "CommonUtility.h"

namespace Necromancer{
	class VSDeclaration{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, declaration_string);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, uid);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, name);
	public:
		~VSDeclaration();
	private:
	};
}

