#pragma once

#include "Container.hpp"

struct _object;

typedef _object PyObject;

typedef PyObject* (*ModuleInitFun)(void);

namespace Necromancer{
	class DefInitModule{
	public:
		DefInitModule(const String&, ModuleInitFun);
	};

	class InitModules{
	public:
		static void register_module(const String&, ModuleInitFun);
		static void init_modules();
	private:
		static Map<String, ModuleInitFun>::Type s_init_map;
	};
}

