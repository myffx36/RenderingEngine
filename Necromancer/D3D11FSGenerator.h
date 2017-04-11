#pragma once

#include "FSGenerator.h"

namespace Necromancer{
	class D3D11FSGenerator:public FSGenerator{
	public:
		D3D11FSGenerator();

		virtual ~D3D11FSGenerator();
	private:
		virtual String generate_input_text(const String&);
		virtual String generate_output_text(U32);
		virtual String generate_header_text(const String& input);
		virtual String generate_tail_text(const String& input);
		virtual String save_debug_file(const String&, const String&);

		void init_buid_in_fs();
	};
}

