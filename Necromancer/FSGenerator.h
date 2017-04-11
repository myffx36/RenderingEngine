#pragma once

#include "RenderingResource.h"
#include "Container.hpp"

namespace Necromancer{
	class FSProcess;
	class FSDeclaration;
	class FSGenerator{
	public:
		FSPtr generate_fragment_shader(const String&, const String& format);

		~FSGenerator();

		void attach_fs_process(FSProcess*);
		void attach_fs_declaration(FSDeclaration*);

		void set_head_common(const String&);
		void set_tail(const String&);
		void set_head_map(char, const String&);
		void set_output_mode(const String&);
		void set_output_head(const String&);
		void set_output_tail(const String&);
		void set_input_head(const String&);
		void set_input_tail(const String&);
		void set_input_map(char, const String&);
		void set_extension(const String&);
	private:
		FSPtr create_fragment_shader(const String&, const String& format);
		String combine_vs_input_and_output(const String&, const String&);
		
		String generate_input_text(const String&);
		String generate_output_text(U32);
		String generate_header_text(const String& input);
		String generate_tail_text(const String& input);
#ifdef _DEBUG
		virtual String save_debug_file(const String&, const String&);
#endif
	private:
		Map<U32, FSProcess*>::Type m_uid_map;
		Map<U32, FSDeclaration*>::Type m_declaration_map;
		Map<String, FSPtr>::Type m_fs_map;
		String m_head_common;
		String m_tail;
		Map<char, String>::Type m_head_map;
		String m_output_mode;
		String m_output_head;
		String m_output_tail;
		String m_input_head;
		String m_input_tail;
		Map<char, String>::Type m_input_map;
		String m_extension;
	};

	const U32 lighting_fs_uid = 0;
	const U32 texture_maping_fs_uid = 1;
	const U32 white_fs_uid = 2;
	const U32 red_fs_uid = 3;
	const U32 vertex_color_fs_uid = 4;
	const U32 lighting_with_shadow_fs_uid = 5;
	const U32 g_buffer_fs_uid = 6;

	const U32 lighting_fs_dec_uid = 0;
	const U32 diffuse_texture_desc_uid = 1; 
	const U32 shadow_map_fs_dec_uid = 2;
	const U32 g_buffer_fs_dec_uid = 3;
}

