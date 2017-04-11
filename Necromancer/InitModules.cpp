#include "stdafx.h"
#include "InitModules.h"

namespace Necromancer{
	Map<String, ModuleInitFun>::Type InitModules::s_init_map;

	void access_init_map(int flag, const String& name, ModuleInitFun fun){
		static Map<String, ModuleInitFun>::Type init_map;
		if(flag == 0){
			for(auto iter = init_map.begin();iter != init_map.end();++ iter){
				PyImport_AppendInittab(iter->first.c_str(), iter->second);
			}
		}else{
			init_map[name] = fun;
		}
	}


	void InitModules::register_module(const String& name, ModuleInitFun fun){
		access_init_map(1, name, fun);
	}

	void InitModules::init_modules(){
		access_init_map(0, "", nullptr);
	}

	DefInitModule::DefInitModule(const String& name, ModuleInitFun fun){
		InitModules::register_module(name, fun);
	}
}