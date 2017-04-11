#ifndef NECROMANCER_CONTAINER_HPP
#define NECROMANCER_CONTAINER_HPP

namespace Necromancer{
	template<typename T>
	struct SmartPtr{
		typedef std::shared_ptr<T> Type;
	};

	template<typename T>
	struct WeakPtr{
		typedef std::weak_ptr<T> Type;
	};

	template<typename T>
	struct Vector{
		typedef std::vector<T> Type;
	};

	template<typename T, size_t size>
	struct Array{
		typedef std::array<T, size> Type;
	};

	template<typename T>
	struct List{
		typedef std::list<T> Type;
	};

	template<typename KT, typename VT>
	struct Map{
		typedef std::map<KT, VT> Type;
	};

	template<typename T>
	struct Stack{
		typedef std::stack<T> Type;
	};

	template<typename T>
	struct Queue{
		typedef std::queue<T> Type;
	};

	typedef std::string String;
	typedef std::wstring WString;
}

#endif