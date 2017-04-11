#ifndef NECROMANCER_COMMON_TEMPLATE_HPP
#define NECROMANCER_COMMON_TEMPLATE_HPP

#include "CommonDataTypes.hpp"

namespace Necromancer{
	class BaseVoidCaller{
	public:
		virtual ~BaseVoidCaller(){

		}
		virtual void operator()() = 0;
	private:
	};

	template<typename T>class VoidCaller:public BaseVoidCaller{
	public:
		VoidCaller(T caller)
			:m_caller(caller)
		{

		}

		virtual void operator()(){
			m_caller();
		}

		virtual ~VoidCaller(){

		}
	private:
		T m_caller;
	};

	class BaseReturnU32Caller{
	public:
		virtual ~BaseReturnU32Caller(){
		}

		virtual U32 operator()(void) = 0;
	};
	template<typename T>class Return32Caller:public BaseReturnU32Caller{
		Return32Caller(T caller)
			:m_caller(caller)
		{
		}

		U32 operator()(){
			return m_caller();
		}

		virtual ~Return32Caller(){
		}
	private:
		T m_caller;
	};
}

#endif