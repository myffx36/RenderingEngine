#ifndef NECROMANCER_COMMON_DATA_TYPES_HPP
#define NECROMANCER_COMMON_DATA_TYPES_HPP

namespace Necromancer{
#ifdef _WIN32
	//16位无符号整型
	typedef unsigned __int16 U16;

	//32位无符号整型
	typedef unsigned __int32 U32;

	//64位无符号整型
	typedef unsigned __int64 U64;

	//16位整型
	typedef __int16 I16;

	//32位整型
	typedef __int32 I32;

	//64位整形
	typedef __int64 I64;

	//32位浮点型
	typedef float F32;

	//64位浮点型
	typedef double F64;
#endif
}

#endif