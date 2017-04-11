#ifndef NECROMANCER_ALGORITHM_HPP
#define NECROMANCER_ALGORITHM_HPP

namespace Necromancer{
	template<typename ContainerType, typename Proc>
	void for_each_do(const ContainerType& container,
		Proc proc)
	{
		typename ContainerType::Iterator begin = container.begin();
		typename ContainerType::Iterator end = container.end();
		typename ContainerType::Iterator iter;
		for(iter = begin;iter != end;++ iter){
			proc(*iter);
		}
	}

	template<typename ContainerType>
	typename ContainerType::Iterator find(
		const ContainerType& container,
		const typename ContainerType::Iterator::ValueType& val)
	{
		typename ContainerType::Iterator begin = container.begin();
		typename ContainerType::Iterator end = container.end();
		typename ContainerType::Iterator iter;
		for(iter = begin;iter != end;++ iter){
			if((*iter) == val) return iter;
		}
		return end;
	}

	template<typename T>
	void swap(T& t1, T& t2){
		T tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
}

#endif