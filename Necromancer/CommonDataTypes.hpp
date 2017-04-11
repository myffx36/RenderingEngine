#ifndef NECROMANCER_COMMON_DATA_TYPES_HPP
#define NECROMANCER_COMMON_DATA_TYPES_HPP

namespace Necromancer{
#ifdef _WIN32
	//16λ�޷�������
	typedef unsigned __int16 U16;

	//32λ�޷�������
	typedef unsigned __int32 U32;

	//64λ�޷�������
	typedef unsigned __int64 U64;

	//16λ����
	typedef __int16 I16;

	//32λ����
	typedef __int32 I32;

	//64λ����
	typedef __int64 I64;

	//32λ������
	typedef float F32;

	//64λ������
	typedef double F64;
#endif
}

#endif