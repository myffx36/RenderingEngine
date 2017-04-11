#pragma once

#include "RenderingResource.h"

namespace Necromancer{
	class VSProcess;
	class VSDeclaration;
	class VSGenerator{
	public:
		VSPtr generate_vertex_shader(const String&, const String& format);

		virtual ~VSGenerator();

		void attach_vs_process(VSProcess*);
		void attach_vs_declaration(VSDeclaration*);

		void set_head(const String& s);
		void set_tail(const String& s);
		void set_input_head(const String& s);
		void set_input_tail(const String& s);
		void set_output_head(const String& s);
		void set_output_tail(const String& s);
		void set_input_map(char c, const String& s);
		void set_output_map(char c, const String& s);
		void set_extension(const String& s);
	private:
		VSPtr create_vertex_shader(const String& vs_code, const String& vb_format);
		String combine_vs_input_and_output(const String&, const String&);
		String generate_input_text(const String&);
		String generate_output_text(const String&);
		String generate_header_text(const String& input, const String& output);
		String generate_tail_text(const String& input, const String& output);
#ifdef _DEBUG
		virtual String save_debug_file(const String&, const String&);
#endif
	private:
		Map<U32, VSProcess*>::Type m_uid_map;
		Map<U32, VSDeclaration*>::Type m_declaration_map;
		Map<String, VSPtr>::Type m_vs_map;
		String m_head;
		String m_tail;
		String m_input_head;
		String m_input_tail;
		String m_output_head;
		String m_output_tail;
		String m_extension;
		Map<char, String>::Type m_input_map;
		Map<char, String>::Type m_output_map;
	};


	const U32 perspective_projection_vs_uid = 0;
	const U32 skining_transform_vs_uid = 1;

	const U32 matrices_declaration_uid = 0;
	const U32 skeleton_declaration_uid = 1;
}

