#include "stdafx.h"
#include "Exception.h"

namespace Necromancer{
	Exception::Exception(const char* str)
		:m_str(str)
	{
	}

	Exception::Exception(const String& str)
		:m_str(str)
	{

	}

	Exception::~Exception(){
	}

	UnsupportOperationException::UnsupportOperationException(const char* str)
		:Exception(str)
	{
	}

	UnsupportOperationException::~UnsupportOperationException(){
	}
}