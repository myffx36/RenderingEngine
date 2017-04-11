#pragma once

#include "RenderingResource.h"

namespace Necromancer{
	class VSGenerator;
	class FSGenerator;
	class VSProcess;
	class FSProcess;
	class VSDeclaration;
	class FSDeclaration;
	class PILShaderManager{
	public:
		PILShaderManager();
		virtual ~PILShaderManager();

		virtual VSPtr create_vertex_shader(const char* vs_code, const char* format);
		virtual FSPtr create_fragment_shader(const char* fs_code, const char* format);

		void attach_fs_process(FSProcess*);
		void attach_vs_process(VSProcess*);
		void attach_vs_declaration(VSDeclaration*);
		void attach_fs_declaration(FSDeclaration*);
	private:
		VSGenerator* m_vs_generator;
		FSGenerator* m_fs_generator;
	};
}

