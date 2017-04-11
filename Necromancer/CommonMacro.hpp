#ifndef NECROMANCER_COMMON_MACRO_HPP
#define NECROMANCER_COMMON_MACRO_HPP

#define DEF_DATA_MEMBER_AND_GETTER_SETTER(TYPE, NAME)\
	private:\
		TYPE m_##NAME;\
	public:\
	inline void set_##NAME(const TYPE & val){\
		m_##NAME = val;\
	}\
	inline const TYPE& NAME() const{\
		return m_##NAME;\
	}

#define DEF_PTR_MEMBER_AND_GETTER_SETTER(TYPE, NAME)\
	private:\
		TYPE* m_##NAME;\
	public:\
	inline void set_##NAME(TYPE* val){\
		m_##NAME = val;\
	}\
	inline TYPE* NAME() const{\
		return m_##NAME;\
	}

#define DEF_DATA_MEMBER_AND_GETTER(TYPE, NAME)\
	private:\
		TYPE m_##NAME;\
	public:\
	inline const TYPE& NAME() const{\
		return m_##NAME;\
	}

#define DEF_PTR_MEMBER_AND_GETTER(TYPE, NAME)\
	private:\
		TYPE* m_##NAME;\
	public:\
	inline TYPE* NAME() const{\
		return m_##NAME;\
	}

#endif