#pragma once

#include "TwoDObject.h"

namespace Necromancer{
	class PictureObject:public TwoDObject{
	public:
		PictureObject(const char*);
		PictureObject(const wchar_t*);

		virtual ~PictureObject();
	};
}

