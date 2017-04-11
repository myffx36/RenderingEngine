#pragma once

namespace Necromancer{
	class Script{
	private:
		void process_exception();
	public:
		Script(const char* ,const char*);

		virtual ~Script();

		void operator()(){
			try{
				m_fun();
			}catch(...){
				process_exception();
				throw;
			}
		}

		template<typename T>
		void operator()(T val){
			try{
				m_fun(val);
			}catch(...){   
				process_exception();
				throw;
			}
		}

		template<typename ReturnType, typename T1>
		ReturnType operator()(T1 p1){
			try{
				ReturnType rst = boost::python::extract<ReturnType>(m_fun(p1));
				return rst;
			}catch(...){   
				process_exception();
				throw;
			}
		}

		template<typename ReturnType, typename T1, typename T2>
		ReturnType operator()(T1 p1, T2 p2){
			try{
				ReturnType rst = boost::python::extract<ReturnType>(m_fun(p1, p2));
				return rst;
			}catch(...){   
				process_exception();
			}
		}

		template<typename ReturnType, typename T1, typename T2, typename T3>
		ReturnType operator()(T1 p1, T2 p2, T3 p3){
			try{
				ReturnType rst = boost::python::extract<ReturnType>(m_fun(p1, p2, p3));
				return rst;
			}catch(...){   
				process_exception();
				throw;
			}
		}
	private:
		boost::python::object m_fun;
	};
}

