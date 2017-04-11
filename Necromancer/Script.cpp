#include "stdafx.h"
#include "Script.h"

#include "Container.hpp"

using namespace boost::python;

namespace{
	std::string handle_pyerror()
	{
		using namespace boost::python;
		using namespace boost;

		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);

		handle<> hType(ptype);
		object extype(hType);
		handle<> hTraceback(ptraceback);
		object traceback(hTraceback);

		//Extract error message
		std::string strErrorMessage = extract<std::string>(pvalue);

		long lineno = extract<long> (traceback.attr("tb_lineno"));
		std::string filename = extract<std::string>(
			traceback.attr("tb_frame").attr("f_code").attr("co_filename"));
		std::string funcname = extract<std::string>(
			traceback.attr("tb_frame").attr("f_code").attr("co_name"));

		return strErrorMessage;
	}
}

namespace Necromancer{

	void Script::process_exception(){
		String text = handle_pyerror();
		MessageBoxA(nullptr, text.c_str(), "exception", MB_OK);
	}

	Script::Script(const char* file_name, const char* fun_name){
		try{
			boost::python::object module = boost::python::import(file_name);
			m_fun = module.attr(fun_name);
		}catch(...){
			PyErr_Print();
			process_exception();
			throw;
		}
	}

	Script::~Script(){

	}
}