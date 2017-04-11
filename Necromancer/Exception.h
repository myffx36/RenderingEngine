#pragma once

#include "Container.hpp"

namespace Necromancer{

	class Exception{
	public:
		Exception(const char*);
		Exception(const String& name);
		virtual ~Exception();

		operator const String&() const{
			return m_str;
		}

		const String& str() const{
			return m_str;
		}
	private:
		String m_str;
	};

	class UnsupportOperationException:public Exception{
	public:
		UnsupportOperationException(const char*);
		virtual ~UnsupportOperationException();
	private:
	};
}

